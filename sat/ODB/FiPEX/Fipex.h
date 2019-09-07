
#ifndef _QB50_FIPEX_H
#define _QB50_FIPEX_H

#include "devices.h"

#define VKI_EPOCH 946684800
#define VKI_NEVER 0xffffffff


namespace qb50 {

   class Fipex : public Device
   {

      public:

         enum CmdId
         {
            SU_PING    = 0x00,
            SU_INIT    = 0x01,
            SU_ID      = 0x04,
            SU_STDBY   = 0x0a,
            SU_SC      = 0x0b,
            SU_SM      = 0x0c,
            OBC_SU_ON  = 0x0f,
            SU_RSP     = 0x10,
            SU_SP      = 0x11,
            SU_HK      = 0x20,
            SU_DP      = 0x21,
            SU_CAL     = 0x33,
            OBC_SU_OFF = 0xf0,
            OBC_SU_END = 0xff
         };

         enum RspId
         {
            SU_R_ACK   = 0x02,
            SU_R_NACK  = 0x03,
            SU_R_ID    = 0x04,
            SU_R_HK    = 0x20,
            SU_R_SDP   = 0x30,
            SU_R_CAL   = 0x33
         };

         struct CmdHeader
         {
            uint8_t  sb;    // start byte
            uint8_t  id;    // command ID
            uint8_t  len;   // payload length
            uint8_t  x[];   // payload
         } __attribute__(( packed ));

         struct RspHeader
         {
            uint8_t  sb;    // start byte
            uint8_t  id;    // response ID
            uint8_t  len;   // payload length
            uint8_t  seq;   // sequence count
            uint8_t  x[];   // payload
         } __attribute__(( packed ));

         #include "Exception.h"
         #include "Script.h"

         Fipex( const char *name, STM32_UART& uart, STM32_GPIO::Pin& enPin );
         virtual ~Fipex();

         Fipex& init    ( void );
         Fipex& enable  ( bool debug = false );
         Fipex& disable ( bool debug = false );

         Fipex& activeScript ( unsigned sn );
         Fipex& storeScript  ( unsigned sn, Script::Header *sh );

         Script::Header *loadScript( unsigned sn );

         size_t send( CmdHeader *ch, RspHeader *rh, int toms );
         size_t recv(                RspHeader *rh, int toms );

       //Fipex& runCommand   ( CmdHeader *ch, RspHeader *rh );
         Fipex& abort        ( void );

         unsigned activeScript ( void ) const
         { return _sn; }


      private:

         STM32_UART&      _uart;
         STM32_GPIO::Pin& _enPin;

         uint8_t *_st;  // scripts table
         uint8_t *_sc;  // active script data
         unsigned _sn;  // acrive script number

   };

   extern qb50::Fipex::ScriptFormatException  FipexScriptFormatException;
   extern qb50::Fipex::ResponseException      FipexResponseException;
   extern qb50::Fipex::TimeoutException       FipexTimeoutException;

   extern qb50::Fipex SU;

} /* qb50 */


#endif /*_QB50_FIPEX_H*/

/*EoF*/
