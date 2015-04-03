
#include <iostream>

#include "application.hh"
#include "teletext-window.hh"
#include "data-fetcher.hh"
#include "bus-time-fetcher.hh"

using namespace std;

Application::Application( int argc, char **argv ) {
}

Application::~Application() {
}

int Application::main() {

  BusTimeFetcher btf(76309);


  if( !btf.fetch() ) {
    cout << "fetch failed" << endl;
    return -1;
  }

  for( int i = 0; i < btf.count(); i++ ) {

    cout 
      << i
      << " " << btf[i].bus
      << " " << btf[i].destination
      << " " << btf[i].time
      << endl;
  }
  


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

  return 0;
  
  TeleTextWindow window;

  cout << "Bus timetable 2.0" << endl;

  if( !window.open() ) {
    cerr << "error: could not open window" << endl;
    return -1;
  }

  window.move_to( 2, 2 );
  window.set_color( 4 );
  window.set_bg( 14 );
  window.puts( "Yeah, BABY!" );

  while( window.is_active() ) {


    window.tick();
  }
  
  return 0;
}
