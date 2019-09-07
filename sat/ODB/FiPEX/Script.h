
#ifndef _QB50_FIPEX_SCRIPT_H
#define _QB50_FIPEX_SCRIPT_H

#ifndef _QB50_FIPEX_H
 #error "can't include Script.h, try Fipex.h instead"
#endif


class Script
{

   public:

      Script();
      virtual ~Script();

      struct Header
      {
         uint8_t  len;   // script length
         uint32_t stime; // start time
         uint16_t rtime; // repeat time
         uint8_t  ccnt;  // command count
         uint8_t  x[];   // payload
      } __attribute__(( packed ));

#if 0
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
#endif

      static time_t   startTime  ( Header *sh );
      static unsigned repeatTime ( Header *sh );
      static unsigned delay      ( CmdHeader    *ch );

      static const char* cmdName ( Fipex::CmdId id );
      static const char* rspName ( Fipex::RspId id );

#if 0
      /* dump command */
      static void     dump       ( Header    *sh );
      /* dump command */
      static void     dump       ( CmdHeader *ch );
#endif

      Script&    load  ( Header *sh, bool debug = false );
      Script&    dump  ( void );
      Script&    reset ( void );
      CmdHeader* next  ( void );


   private:

      Header *_sh;

      unsigned _cind; // current command index
      unsigned _coff; // current command offset

};


#endif /*_QB50_FIPEX_SCRIPT_H*/

/*EoF*/
