
#include "system/SimpleThread.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

SimpleThread::SimpleThread( const char *name, ThreadWorker worker )
   : Thread( name, 2, RUNNING ), _worker( worker )
{ ; }


SimpleThread::~SimpleThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void SimpleThread::run( void )
{ _worker( this ); }


/*EoF*/
