
#include <curl/curl.h>

#include "data-fetcher.hh"

using std::string;
using std::list;

size_t data_fetcher_writer_callback( void* out_ptr, size_t size, size_t nmem, void* u_data) {

  size_t byte_len  = size * nmem;
  char *str        = (char*)out_ptr;
  DataFetcher *dfp = (DataFetcher *)u_data;

  dfp->write_string( string( str, byte_len ));

  return byte_len; 
}

DataFetcher::DataFetcher() { }

void DataFetcher::write_string( const string &str ) {

  m_chunks.push_back( str );
}

bool DataFetcher::fetch( const char* url ) {

  CURL *curl_handle;
  CURLcode res;
  bool okay = true;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt( curl_handle, CURLOPT_URL, url );
  
  curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, data_fetcher_writer_callback );

  curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, this );

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);
  if(res != CURLE_OK) okay = false;

  curl_easy_cleanup(curl_handle);

  curl_global_cleanup();

  return okay;
}

const string & DataFetcher::result() {


  if( m_data == "" )
    for( list<string>::iterator it = m_chunks.begin(); it != m_chunks.end(); it++ )
      m_data += *it;

  return m_data; 
}
