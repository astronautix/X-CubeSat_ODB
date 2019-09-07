
#include "NRZI.h"

using namespace qb50;


NRZI::NRZI()
{ _prv = 0; }


NRZI::~NRZI()
{ ; }


bool NRZI::push( bool cur )
{
   _prv = !( _prv ^ cur );
   return _prv;
}

/*EoF*/
