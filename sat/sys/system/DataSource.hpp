
#ifndef _QB50_SYSTEM_DATA_SOURCE_HPP
#define _QB50_SYSTEM_DATA_SOURCE_HPP


namespace qb50 {

template <class T>
class DataSource
{

   public:

      DataSource()
      { ; }

      virtual ~DataSource()
      { ; }

      virtual size_t read( T* dst, size_t n, int toms = -1 ) = 0;

};

} /*qb50*/


#endif /* _QB50_SYSTEM_DATA_SOURCE_HPP */
