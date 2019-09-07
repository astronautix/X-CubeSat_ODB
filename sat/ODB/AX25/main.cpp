/**
 *  AX25_test.cpp
 *  28/04/2015
 */


#include "AX25.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>


using namespace qb50;


/**
 * AX25_PacketExampleThread
 *
 * Test AX25 thread
 * @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
 * @version    1.9             (24/04/2015 - 28/04/2015)
 */
void AX25_PacketExampleThread( Thread *self )
{
for( ;; ) {
    PacketAX25 PacketAX25;

    // Define destination
    AX25AddressField destinationField;
    destinationField.CCR = 0x03;
    destinationField.SSID = 0;
    uint8_t destinationAddress[]= "TELEM";
    destinationField.address= destinationAddress;
    PacketAX25.destination = destinationField;

    // Define source
    AX25AddressField sourceField;
    sourceField.CCR = 0x03;
    sourceField.SSID = 1;
    uint8_t sourceAddress[]= "F6FAO";
    sourceField.address= sourceAddress;
    PacketAX25.source = sourceField;

    // Define control and PID
    PacketAX25.controle= AX25_control_Mode::UI;
    PacketAX25.PID= AX25_PID_Mode::NO_LAYER_3;

    // Define information/data
    uint8_t AX25Data[]="Lorem ipsum dolor sit posuere.";
    size_t AX25Data_len = (size_t)30;
/*
    AX25Data[0] = 0xFF; // Bit stuffing Test
    AX25Data[1] = 0xFF; // Bit stuffing Test
    AX25Data[30]= 0xFF; // Bit stuffing Test
*/

    // Memory receive packet
    uint8_t *contentPacket = new uint8_t[120];
    uint8_t *packetSend = new uint8_t[200];
    for(int i=0; i<200; ++i)
    {
        contentPacket[i]= 0;
        packetSend[i]= 0;
    }

    // Write content AX25 in contentPacket
    size_t contentPacket_len = PacketAX25.writeContent(contentPacket, AX25Data, (size_t)AX25Data_len);

    // Write packet AX25 in packet
    sizeBitStuffing_t packetSend_len;
    PacketAX25.writePacket(packetSend, &packetSend_len, contentPacket, contentPacket_len);

    // Disable warning
    (void)self;
    (void)packetSend_len;

    delay( 500 );
}
}


/*EoF*/
