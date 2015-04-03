
#include <sstream>
#include <iostream>

#include "bus-time-fetcher.hh"
#include "data-fetcher.hh"

#include "json.h"

using namespace std;

static const char *s_fetcher_path = "http://countdown.tfl.gov.uk/stopBoard/";

static const char *s_fake_data = "{\"lastUpdated\":\"22:59\",\"filterOut\":[],\"arrivals\":[{\"routeId\":\"48\",\"routeName\":\"48\",\"destination\":\"London Bridge\",\"estimatedWait\":\"4 min\",\"scheduledTime\":\"22:04\",\"isRealTime\":true,\"isCancelled\":false},{\"routeId\":\"55\",\"routeName\":\"55\",\"destination\":\"Oxford Circus\",\"estimatedWait\":\"8 min\",\"scheduledTime\":\"22:08\",\"isRealTime\":true,\"isCancelled\":false},{\"routeId\":\"55\",\"routeName\":\"55\",\"destination\":\"Oxford Circus\",\"estimatedWait\":\"10 min\",\"scheduledTime\":\"22:09\",\"isRealTime\":true,\"isCancelled\":false},{\"routeId\":\"48\",\"routeName\":\"48\",\"destination\":\"London Bridge\",\"estimatedWait\":\"21 min\",\"scheduledTime\":\"22:20\",\"isRealTime\":true,\"isCancelled\":false},{\"routeId\":\"55\",\"routeName\":\"55\",\"destination\":\"Oxford Circus\",\"estimatedWait\":\"23 min\",\"scheduledTime\":\"22:23\",\"isRealTime\":true,\"isCancelled\":false}],\"serviceDisruptions\":{\"infoMessages\":[\"Routes 10 25 55 73 98 390 N8 N55 N73 N98 N207 are subject to diversion until Monday 20 April due to road works on Oxford Street eastbound between Newman Street and Rathbone Place. \",\"Central Line trains are not stopping at Tottenham Court Road. Use Holborn or Oxford Circus stations instead.\"],\"importantMessages\":[],\"criticalMessages\":[]}}";

BusTimeFetcher::BusTimeFetcher( int id ) {

  stringstream ss;
  ss << s_fetcher_path << id;

  m_fetch_url = ss.str();

  cout << "BusTimeFetcher::BusTimeFetcher()" << endl;
  cout << "  url=" << m_fetch_url << endl;
}

bool BusTimeFetcher::fetch() {

  //DataFetcher df;
  
  //if( !df.fetch( m_fetch_url.c_str() )) {
  //  return false;
 // }

  //cout << df.result() << endl;

  Json::Reader json_reader;

  Json::Value root;
  if( !json_reader.parse( s_fake_data, root )) 
    return false;

  // here be data ...
  if( root.isObject() ) {

    cout << "root is object" << endl;

    if( root.isMember( "arrivals" )) {

      cout << "root has arrivals" << endl;

      Json::Value &arrivals = root["arrivals"];

      if( arrivals.isArray()) {

        cout << "arrivals is an array" << endl;

        int count = arrivals.size();
        cout << "arrivals has " << count << " elements" << endl;

        for( int i = 0; i < count; i++ ) {

          Json::Value &arrival = arrivals[i];

          if( arrival.isObject()  ) {

            cout << "arrival[" << i << "] is an object" << endl;

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

            cout << "  has " << values << " values" << endl;
            if( values == 3 ) 
              m_bus_times.push_back(bus_time);
          }
        }

      }
    }
  }




  return true;
}

int BusTimeFetcher::count() {
  return m_bus_times.size();
}

BusTimeFetcher::T_BUS_TIME & BusTimeFetcher::operator[]( int i ) {
  return m_bus_times[i];
}

