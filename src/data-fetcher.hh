
#pragma once

#include <stdlib.h>

#include <string>
#include <list>

class DataFetcher {
private:

  std::list<std::string> m_chunks;
  std::string m_data;
  
  void write_string( const std::string& );
  
  friend size_t data_fetcher_writer_callback( void*, size_t, size_t, void* );

public:

  DataFetcher();

  bool fetch( const char* );
  
  const std::string & result();
};

