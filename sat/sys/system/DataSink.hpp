
#ifndef _QB50_SYSTEM_DATA_SINK_HPP
#define _QB50_SYSTEM_DATA_SINK_HPP


namespace qb50 {

template <class T>
class DataSink
{

   public:

      DataSink()
      { ; }

      virtual ~DataSink()
      { ; }

      virtual size_t write( const T* src, size_t n, int toms = -1 ) = 0;

};

} /*qb50*/


#endif /* _QB50_SYSTEM_DATA_SINK_HPP */
