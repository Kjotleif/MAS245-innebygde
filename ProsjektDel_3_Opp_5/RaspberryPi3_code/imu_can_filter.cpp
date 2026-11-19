#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <iomanip>

// ID til CAN meldinger
constexpr uint32_t rawAngleCanId = 0x100;       // Oppretter en const (i HEX form) med ID til meldingen som skal mottas
constexpr uint32_t filteredAngleCanId = 0x101;  // Oppretter en const (i HEX form) med ID til meldingen som skal sendes

// Alpha bestemmer filtreringen. Mindre alpha -> større filtrering
constexpr float alpha = 0.1f;


// FUNKSJONSDEKLARASJONER

// Rå vinkel gjøres om til tre byte: fortegn, to sifer før koma, to sifer etter koma
void encodeAngle(float angle, struct can_frame &frame);

// Tre byte (fortegn, to sifer før koma, to sifer etter koma) gjøres om tilbake til en vinkel
float decodeAngle(const struct can_frame &frame);

int main()
{
    // Setter opp hastighet på CAN komunikasjon til 250 kbit/s
    std::system("/sbin/ip link set can0 down");
    std::system("/sbin/ip link set can0 up type can bitrate 250000");

    // Oppretter can_frame-objekt. Mottatte meldinger blir lagret i den.
    struct can_frame receivedFrame {};
    // Oppretter can_frame-objekt. Den skal innholde meldinger vi vil sende.
    struct can_frame responseFrame {};

    float filteredAngle = 0.0f;
    bool firstMeasurement = true;

    // Oppretter en rå CAN-socket
    int canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Lagrer navnet til CAN-grensesnittet
    struct ifreq interfaceRequest {};
    strncpy(interfaceRequest.ifr_name, "can0", IFNAMSIZ - 1);

    // Finner indeksnummeret til can0
    ioctl(canSocket, SIOCGIFINDEX, &interfaceRequest);

    // Oppretter adressen til CAN-grensesnittet
    struct sockaddr_can address {};
    address.can_family = AF_CAN;
    address.can_ifindex = interfaceRequest.ifr_ifindex;

    // Kobler CAN-channel til can0
    bind(canSocket, reinterpret_cast<struct sockaddr *>(&address), sizeof(address));


    while (true)
    {
        // Leser can_frame fra Teensy
        read(canSocket, &receivedFrame, sizeof(receivedFrame));

        // Enkel sjekk at melding som ble motatt er riktig.
        if (receivedFrame.can_id != rawAngleCanId || receivedFrame.can_dlc != 3)
        {
            continue;
        }

        // Mottat melding blir decodet fra 3 byte til en vinkel med fortegn.
        const float rawAngle = decodeAngle(receivedFrame);

        // Lavpasfilter funker feil uten initielverdi. Denne if-else statement gjør slik at
        // første filtrert vinkel er lik rå vinkel. Så brukes den initielverdi videre til filtrering.
        if (firstMeasurement)
        {
            filteredAngle = rawAngle;
            firstMeasurement = false;
        }
        else
        {
            // Filtrerer vinkelen ved hjelp av lavpassfilter.
            filteredAngle = alpha * rawAngle + (1.0f - alpha) * filteredAngle;
        }

        // Den filtrerte vinkelen deles opp i tre byte: fortegn, første to siffer og siste to siffer
        // og lagres inn i response frame
        encodeAngle(filteredAngle, responseFrame);

        // Sender can_frame til Teensy
        write(canSocket, &responseFrame, sizeof(responseFrame));

        // Skriver ut i RPi terminaler
        std::cout << std::fixed << std::setprecision(2) << "Raw: " << rawAngle << " deg" << " | Filtered: " << filteredAngle << " deg" << std::endl;
    }

    close(canSocket); // Lukker CAN socket
    return 0;
}



// FUNKSJONSDEFINISJONER
void encodeAngle(float angle, struct can_frame &frame)
{
    frame.can_id = filteredAngleCanId;
    frame.can_dlc = 3;

    // Sjekker fortegnet til vinkelen og lagrer byte nr. 1
    if (angle >= 0.0f)
    {
        frame.data[0] = 1;
    }
    else
    {
        frame.data[0] = 0;
    }

    float absValAngle = fabsf(angle); // Oppretter en variabel med absoluttverdien av vinkelen


    uint16_t angle_x_100 = static_cast<uint16_t>(roundf(absValAngle * 100.0f));   // 12.34 -> 1234

    frame.data[1] = static_cast<uint8_t>(angle_x_100 / 100);    // Lagrer byte nr. 2.   1234 / 100 = 12
    frame.data[2] = static_cast<uint8_t>(angle_x_100 % 100);    // Lagrer byte nr. 3.   1234 % 100 = 34 (Resten av 1234 / 100)
}

float decodeAngle(const struct can_frame &frame)
{
    // Setter sammen byte nr. 2 og byte nr. 3:
    // 12 * 100 + 34 = 1234
    int16_t vinkel = frame.data[1] * 100 + frame.data[2];

    // Hvis byte nr. 1 er 0, er vinkelen negativ
    if (frame.data[0] == 0)
    {
        vinkel = -vinkel;
    }

    return static_cast<float>(vinkel) / 100.0f; // Deler på 100.    1234 / 100 = 12.34
}