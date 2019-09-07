
#include "Event.h"

using namespace qb50;


const char *Event::events[] = {
   "GENERIC",
   "AD_SUCCESS", "AD_FAILURE", "VBAT_HIGH",  "VBAT_LOW",   "PA_HIGH",
   "PA_LOW",     "FORM",       "WOD_EMPTY",  "(unknown)",  "(unknown)",
   "CMD_1",      "CMD_2",      "CMD_3",      "CMD_4",      "CMD_5",
   "CMD_6",      "CMD_7",      "CMD_8",      "CMD_9",      "CMD_10",
   "CMD_11",     "CMD_12",     "CMD_13",     "CMD_14",     "CMD_15"
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Event::Event( event_t type, Form *form )
   : type( type ), form( form )
{ ; }


Event::~Event()
{
   if(( type == FORM ) && ( form != 0 ))
      delete form;
}


/*EoF*/
