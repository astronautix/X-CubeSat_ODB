
#ifndef _QB50_ODB_COMMAND_THREAD_H
#define _QB50_ODB_COMMAND_THREAD_H

#include "system/qb50.h"
#include "Form.h"
#include "Event.h"


namespace qb50 {

   class CommandThread : public Thread
   {

      public:

         CommandThread();
         ~CommandThread();

         void run( void );

      protected:

         int _parseForm     ( void );

         int _parseCForm    ( void );
         int _parseHForm    ( void );
         int _parsePForm    ( void );
         int _parseFForm    ( void );
#if 0
         int _parseTForm    ( void );
         int _parseT1Form   ( void );
         int _parseT2Form   ( void );
#endif

         int _parseHex      ( uint8_t *x, size_t len );
         int _parseInteger  ( long    &p );
         int _parseDecimal  ( double  &p );
         int _parseDouble   ( double  &p );
         int _parseSign     ( int     &p );
#if 0
         int _parseNoradSci ( double &p, size_t off, size_t len );
         int _parseNoradExp ( double &p, size_t off, size_t len );
#endif
         int _checkTLESum   ( const uint8_t *x, size_t& off, size_t n );

         uint8_t       *_c;
         uint8_t       *_x;
         mBuf<uint8_t>  _m;

         Form _form;

   };

} /* qb50 */


#endif /*_QB50_ODB_COMMAND_THREAD_H*/

/*EoF*/
