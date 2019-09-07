/**
 *  AX25.h
 *  27/04/2015
 */


#ifndef _QB50_AX25_H_
#define _QB50_AX25_H_


#include "system/qb50.h"
#include "bitStuffing.h"


namespace qb50
{


    /**
     *  AX25_PID_Mode
     *
     *  Protocol Identifier (PID) field enumerate.
     *  All layer 3 implemented are not implemented!
     *
     *  ╔═══════╦═══════════╦═══════════════════════════════════════════════════════════════════════════╗
     *  ║  HEX  ║ MSB  LSB  ║                                Translation                                ║
     *  ╠═══════╬═══════════╬═══════════════════════════════════════════════════════════════════════════╣
     *  ║ 0x01  ║ 00000001  ║ ISO 8208/CCITT X.25 PLP                                                   ║
     *  ║ 0x06  ║ 00000110  ║ Compressed TCP/IP packet. Van Jacobson (RFC 1144)                         ║
     *  ║ 0x07  ║ 00000111  ║ Uncompressed TCP/IP packet. Van Jacobson (RFC 1144)                       ║
     *  ║ 0x08  ║ 00001000  ║ Segmentation fragment                                                     ║
     *  ║ **    ║ yy01yyyy  ║ AX.25 layer 3 implemented.                                                ║
     *  ║ **    ║ yy10yyyy  ║ AX.25 layer 3 implemented.                                                ║
     *  ║ 0xC3  ║ 11000011  ║ TEXNET datagram protocol                                                  ║
     *  ║ 0xC4  ║ 11000100  ║ Link Quality Protocol                                                     ║
     *  ║ 0xCA  ║ 11001010  ║ Appletalk                                                                 ║
     *  ║ 0xCB  ║ 11001011  ║ Appletalk ARP                                                             ║
     *  ║ 0xCC  ║ 11001100  ║ ARPA Internet Protocol                                                    ║
     *  ║ 0xCD  ║ 11001101  ║ ARPA Address resolution                                                   ║
     *  ║ 0xCE  ║ 11001110  ║ FlexNet                                                                   ║
     *  ║ 0xCF  ║ 11001111  ║ NET/ROM                                                                   ║
     *  ║ 0xF0  ║ 11110000  ║ No layer 3 protocol implemented.                                          ║
     *  ║ 0xFF  ║ 11111111  ║ Escape character. Next octet contains more Level 3 protocol information.  ║
     *  ╚═══════╩═══════════╩═══════════════════════════════════════════════════════════════════════════╝
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.0             (23/04/2015)
     */
    enum AX25_PID_Mode
    {
        ISO                     = 0x01,
        COMPRESSED_TCP_IP       = 0x06,
        UNCOMPRESSED_TCP_IP     = 0x07,
        SEGMENTATION            = 0x08,
        TEXNET                  = 0xC3,
        LINK                    = 0xC4,
        APPLETALK               = 0xCA,
        APPLETALK_ARP           = 0xCB,
        ARPA_INTERNET_PROTOCOL  = 0xCC,
        ARPA_ADDRESS_RESOLUTION = 0xCD,
        FLEXNET                 = 0xCE,
        NET_ROM                 = 0xCF,
        NO_LAYER_3              = 0xF0,
        ESCAPE_CHARACTER        = 0xFF,
    };


    /**
     *  AX25_control_Mode
     *
     *  Control bits field enumerate.
     *
     * ╔══════╦═════════╦══════════════╦═════╦══════╦═════════════════╦═══╦═══╦═════════╗
     * ║  7   ║    6    ║      5       ║  4  ║  3   ║         2       ║ 1 ║ 0 ║         ║
     * ╠══════╩═════════╩══════════════╬═════╬══════╩═════════════════╩═══╬═══╬═════════╣
     * ║   N(R) Receive sequence no.   ║ P/F ║   N(S) Send sequence no.   ║ 0 ║ I-frame ║
     * ╠═══════════════════════════════╬═════╬════════════════════════╦═══╬═══╬═════════╣
     * ║   N(R) Receive sequence no.   ║ P/F ║          type          ║ 0 ║ 1 ║ S-frame ║
     * ╠═══════════════════════════════╬═════╬════════════════════════╬═══╬═══╬═════════╣
     * ║            type               ║ P/F ║          type          ║ 1 ║ 1 ║ U-frame ║
     * ╚═══════════════════════════════╩═════╩════════════════════════╩═══╩═══╩═════════╝
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.0             (24/04/2015)
     */
    enum AX25_control_Mode
    {
        I   = 0x00,
        S   = 0x01,
        U   = 0x03,
        UI  = 0x03,
    };


    /**
     *  AX25AddressField
     *
     *  Define address, SSID and CCR
     *
     *  Address:  Allow character is A to Z (no a-z), 0 to 9 and space
     *            Other character is replace by space
     *  SSID:     Number 0 to 15
     *  CCR:      Define CCR of address, allowed value: "000" to "111" (0 to 7)
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.0             (24/04/2015)
     */
    struct AX25AddressField
    {
        uint8_t *address;
        uint8_t SSID;
        uint8_t CCR;
    };


    /**
     *  PacketAX25
     *
     *  source/destination/control/PID define header content of AX.25 protocol
     *  writeHeader/writeInformation make element Packet AX.25
     *  writeContent make complete Packet AX.25
     *
     *  Description of methods is in "AX25.cpp" file
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.0             (24/04/2015 - 27/04/2015)
     */
    class PacketAX25
    {


        public:
            /**
             *  Headers parameters
             */
             // Define address
            AX25AddressField source;
            AX25AddressField destination;

            uint8_t controle;   // Define control of packet (look AX25_control_Mode)
            uint8_t PID;        // Define PID of packet (look AX25_PID_Mode)


            /**
             *  Methods
             */
            size_t writeContent(uint8_t *dst, const uint8_t *src, size_t src_len);
            void   writePacket (uint8_t *dst, sizeBitStuffing_t *dst_len, const uint8_t *src, size_t src_len);


        private:
            size_t writeHeader(uint8_t *dst);
            size_t writeInformation(uint8_t *dst, const uint8_t *src, size_t src_len);


    };


} /* namespace: qb50 */


#endif /* _QB50_AX25_H_ */


/*EoF*/
