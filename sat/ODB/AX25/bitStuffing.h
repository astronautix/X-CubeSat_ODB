/**
 *  bitStuffing.h
 *  04/05/2015
 */


#ifndef _QB50_BITSTUFFING_H_
#define _QB50_BITSTUFFING_H_


#include "system/qb50.h"


namespace qb50
{


    /**
     *  sizeBitStuffing_t
     *
     *  ByteSize: Number of data bytes
     *  BitSize: Number of data bits
     *  LastBitLocation: Last bit write 1 to 8
     *
     *  Example:
     *   ╔═════╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦════════════════════╗
     *   ║ bit ║ 7 ║ 6 ║ 5 ║ 4 ║ 3 ║ 2 ║ 1 ║ 0 ║                    ║
     *   ╠═════╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬════════════════════╣
     *   ║     ║ X ║ X ║ X ║ X ║ X ║ X ║ X ║ S ║ LastBitLocation= 1 ║
     *   ║     ║ x ║ x ║ X ║ X ║ X ║ S ║ S ║ S ║ LastBitLocation= 4 ║
     *   ║     ║ S ║ S ║ S ║ S ║ S ║ S ║ S ║ S ║ LastBitLocation= 8 ║
     *   ╚═════╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩════════════════════╝
     *
     *   S= bit set in last byte    X= unknown bit (is not set)
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.1             (04/05/2015 - 06/05/2015)
     */
    struct sizeBitStuffing_t
    {
        size_t      ByteSize=0;
        uint16_t    BitSize=0;
        uint8_t     LastBitLocation=0;
    };


    /**
     *  bitStuffing
     *
     *  Bit stuffing function in LSB first order.
     *  Write '0' value after five contiguous '1' bits.
     *
     *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
     *  @version    1.2             (04/05/2015 - 06/05/2015)
     */
    extern void bitStuffing(uint8_t *dst, sizeBitStuffing_t *dst_len, const uint8_t *src, size_t src_len);


}; /* namespace: qb50 */


#endif /* _QB50_BITSTUFFING_H_ */


/*EoF*/
