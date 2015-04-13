
#include <iostream>
#include <time.h>

#include "application.hh"
#include "teletext-window.hh"
#include "data-fetcher.hh"
#include "bus-time-fetcher.hh"

using namespace std;

Application::Application( int argc, char **argv ) {
}

Application::~Application() {
}

const char *month_short_str [] = {
  "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

int Application::main() {

  char buffer[41];


//  DataFetcher df;
//
//  const char *url = "http://query.yahooapis.com/v1/public/yql?q=select+item+from+weather.forecast+where+location=\"48907\"&format=json";
//
//  if( df.fetch( url )) {
 //    cout << "fetch OKAY" << endl;
//  } else {
//    cout << "fetch FAILED" << endl;
//    return -1;
//  }
//
//  cout << df.result() << endl << endl << endl;

//  return 0;
  
  TeleTextWindow window;

  cout << "Bus timetable 2.0" << endl;

  if( !window.open() ) {
    cerr << "error: could not open window" << endl;
    return -1;
  }

  BusTimeFetcher btf1( 76309, "West bound (To Old Street)" );
  BusTimeFetcher btf2( 48630, "East Bound" );


  if( !btf1.fetch() ) {
    cout << "fetch failed" << endl;
    return -1;
  }

  if( !btf2.fetch() ) {
    cout << "fetch failed" << endl;
    return -1;
  }

  struct tm now;
  time_t now_time;
  int item_count;

  while( window.is_active() ) {

    window.move_to( 0, 24 );
    window.set_color( TC_BLUE );
    window.set_bg( TC_BLACK );
    window.puts( "----------------------------------------" );

    time(&now_time);
    gmtime_r( &now_time, &now );

    snprintf( buffer, 41, 
              "P201  TextSpace   %02d-%3s-%02d     %02d:%02d:%02d",
              now.tm_mday,
              month_short_str[now.tm_mon],
              now.tm_year - 100,
              now.tm_hour,
              now.tm_min,
              now.tm_sec);

    window.set_color( TC_BLACK );
    window.set_bg( TC_YELLOW );
    window.move_to( 0, 0 );
    window.puts( buffer );


    item_count = btf1.count();
    if( item_count > 8 ) item_count = 8;

    window.set_color( TC_GREEN );
    window.set_bg( TC_BLACK );

    window.move_to( 2, 3 );
    window.puts( btf1.name() );

    for( int i = 0; i < item_count; i++ ) {

      snprintf( buffer, 41, "%4s %20s %10s", btf1[i].bus.c_str(), btf1[i].destination.c_str(), btf1[i].time.c_str() );

      window.set_color( (i&1) ? TC_YELLOW : TC_MAGENTA );
      window.move_to( 2, 4 + i );
      window.puts( buffer );
    }

    item_count = btf2.count();
    if( item_count > 8 ) item_count = 8;

    window.move_to( 2, 12 );
    window.set_color( TC_GREEN );
    window.puts( btf2.name() );

    for( int i = 0; i < item_count; i++ ) {

      snprintf( buffer, 41, "%4s %20s %10s", btf2[i].bus.c_str(), btf2[i].destination.c_str(), btf2[i].time.c_str() );

      window.set_color( (i&1) ? TC_YELLOW : TC_MAGENTA );
      window.move_to( 2, 13 + i );
      window.puts( buffer );
    }

  
    window.tick();
  }
  
  return 0;
}
