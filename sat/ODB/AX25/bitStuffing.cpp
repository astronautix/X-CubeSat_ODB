/**
 *  bitStuffing.cpp
 *  04/05/2015
 */


#include "bitStuffing.h"


using namespace qb50;


/**
 *  bitStuffing
 *
 *  Bit stuffing function in LSB first order.
 *  Write '0' value after five contiguous '1' bits.
 *
 *  @author     Jérôme Skoda    <jerome.skoda@hotmail.fr>
 *  @version    1.2             (04/05/2015 - 06/05/2015)
 */
void qb50::bitStuffing(uint8_t *dst, sizeBitStuffing_t *dst_len, const uint8_t *src, size_t src_len)
{
    uint32_t buffer    = 0x00000000;
    uint8_t  oneCounter= 0;
    uint8_t  rightShift= 0;
    uint8_t  cursor    = 0;

    for(uint8_t cursorData= 0; cursorData < src_len; cursorData++)
    {
        uint8_t valueRead= (((uint8_t*)src)[cursorData]);

        /**
         *   Check bit-to-bit (LSB first order) in valueRead
         */
        for(uint16_t msk=0x01; msk <= 0x80; msk=msk<<1)
        {
            if(valueRead & msk)
            {
                // '1' case:
                buffer |=  msk << rightShift; // Write '1' in buffer
                oneCounter++;                 //Increment counter

                // Bit stuffing case
                if(oneCounter >= 5)
                {
                    rightShift++;   // Add zero (Shift right data increment)
                    oneCounter=0;   // Reinitialize counter
                }
            }
            else
            {
                // '0' case:
                /*  Is not necessary because is '0' default
                    buffer &=  ~msk << rightShift; // Write '0' in buffer
                */
                oneCounter=0;   // Reinitialize counter
            }
        }

        //
        ((uint8_t*)dst)[cursor]= (uint8_t)buffer;
        cursor++;
        buffer = buffer >> 8;

        // If number of bit stuffing add octet
        while(rightShift >= 8)
        {
            ((uint8_t*)dst)[cursor]= (uint8_t)buffer;
            cursor++;
            buffer = buffer >> 8;
            rightShift= rightShift-8;
        }
    }

    // Write incomplete octet if is exist
    if(rightShift > 0)
    {
        ((uint8_t*)dst)[cursor]= (uint8_t)buffer;
        cursor++;
    }

    dst_len->BitSize= 8*cursor - 8 + (rightShift?rightShift:8);
    dst_len->ByteSize= cursor;
    dst_len->LastBitLocation= (rightShift?rightShift:8);
}


/*EoF*/
