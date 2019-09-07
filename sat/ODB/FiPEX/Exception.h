
#ifndef _QB50_FIPEX_EXCEPTION_H
#define _QB50_FIPEX_EXCEPTION_H

#ifndef _QB50_FIPEX_H
 #error "can't include Exception.h, try Fipex.h instead"
#endif

#include <exception>
#include <stdexcept>


class Exception : public std::runtime_error
{

   public:

      Exception( const char *msg )
         : std::runtime_error( msg )
      { ; }

};


class TimeoutException : public Exception
{

   public:

      TimeoutException()
         : Exception( "Fipex I/O timeout exception" )
      { ; }

};


class ScriptFormatException : public Exception
{

   public:

      ScriptFormatException()
         : Exception( "Fipex script format exception" )
      { ; }

};


class ResponseException : public Exception
{

   public:

      ResponseException()
         : Exception( "Fipex response exception" )
      { ; }

};


#endif /*_QB50_FIPEX_EXCEPTION_H*/

/*EoF*/
