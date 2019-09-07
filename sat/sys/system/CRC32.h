
#ifndef _QB50_SYSTEM_CRC32_H
#define _QB50_SYSTEM_CRC32_H

#include <cstdint>


namespace qb50 {

   extern uint32_t crc32( const uint8_t *x, size_t len, uint32_t crc = 0 );

} /* qb50 */


#endif /*_QB50_SYSTEM_CRC32_H*/

/*EoF*/
