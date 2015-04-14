
#include <sstream>
#include <iostream>

#include "bus-time-fetcher.hh"
#include "data-fetcher.hh"

#include "json.h"

using namespace std;

static const char *s_fetcher_path = "http://countdown.tfl.gov.uk/stopBoard/";

BusTimeFetcher::BusTimeFetcher( int id, const char* n ) {

  stringstream ss;
  ss << s_fetcher_path << id;

  m_fetch_url = ss.str();
  m_name = n;

//  cout << "BusTimeFetcher::BusTimeFetcher()" << endl;
//  cout << "  url=" << m_fetch_url << endl;
//  cout << " name=" << m_name << endl;
}

bool BusTimeFetcher::fetch() {

  vector<T_BUS_TIME> new_times;

  DataFetcher df;
  
  if( !df.fetch( m_fetch_url.c_str() )) {
    return false;
  }

  //cout << df.result() << endl;

  Json::Reader json_reader;

  Json::Value root;
  if( !json_reader.parse( df.result(), root )) 
    return false;

  // here be data ...
  if( root.isObject() ) {
//    cout << "root is object" << endl;

    if( root.isMember( "arrivals" )) {
//      cout << "root has arrivals" << endl;

      Json::Value &arrivals = root["arrivals"];

      if( arrivals.isArray()) {
//        cout << "arrivals is an array" << endl;

        int count = arrivals.size();
//        cout << "arrivals has " << count << " elements" << endl;

        for( int i = 0; i < count; i++ ) {

          Json::Value &arrival = arrivals[i];

          if( arrival.isObject()  ) {
//            cout << "arrival[" << i << "] is an object" << endl;

            T_BUS_TIME bus_time;
            int values = 0;

            if( arrival.isMember("routeId")) { 
              bus_time.bus = arrival["routeId"].asString();
              values++;
            }

            if( arrival.isMember( "destination" )) {
              bus_time.destination = arrival["destination"].asString();
              values++;
            }

            if( arrival.isMember( "estimatedWait" )) {
              bus_time.time = arrival["estimatedWait"].asString();
              values++;
            }

//            cout << "  has " << values << " values" << endl;
            if( values == 3 ) 
              new_times.push_back(bus_time);
          }
        }

      }
    }
  }

  m_bus_times.assign( new_times.begin(), new_times.end() );

  return true;
}

int BusTimeFetcher::count() {
  return m_bus_times.size();
}

const BusTimeFetcher::T_BUS_TIME & BusTimeFetcher::operator[]( int i ) {
  return m_bus_times[i];
}

const string & BusTimeFetcher::name() {
  return m_name;
}
