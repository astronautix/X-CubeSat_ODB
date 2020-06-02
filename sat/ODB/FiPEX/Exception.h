/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:53+02:00
 * @License: This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * @Copyright: Copyright (C) 2027 Olivier Piras  
 */
 
 
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
