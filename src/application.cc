
#include <iostream>
#include <time.h>
#include <pthread.h>

#include "application.hh"
#include "data-fetcher.hh"

using namespace std;

const char *month_short_str [] = {
  "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

Application::Application( int argc, char **argv ) : 
  m_btf1( 76309, "West bound (To Old Street)" ), 
  m_btf2( 48630, "East Bound" ) {
}

Application::~Application() {
}

void* application_fetcher_thread( void *d ) {
  Application *app = (Application*)d;

  struct timespec ts;

  ts.tv_sec  = 5;
  ts.tv_nsec = 0;

  cout << "fetcher thread started" << endl;

  while(true) {

    app->m_btf1.fetch();
    if( !app->m_run_thread ) break;

    nanosleep( &ts, NULL );

    app->m_btf2.fetch();
    if( !app->m_run_thread ) break;

    nanosleep( &ts, NULL );
  }

  cout << "fetcher thread stopped" << endl;

  return NULL;
}

void Application::print_btf( int pos, TeleTextWindow &window, BusTimeFetcher &btf ) {

  char buffer[41]; 

  int item_count = btf.count();
  if( item_count > 7 ) item_count = 7;

  window.set_color( TC_GREEN );
  window.set_bg( TC_BLACK );

  window.move_to( 2, pos );
  window.puts( btf.name() );

  for( int i = 0; i < item_count; i++ ) {

    snprintf( buffer, 41, "%4s %20s %10s", btf[i].bus.c_str(), btf[i].destination.c_str(), btf[i].time.c_str() );

    window.set_color( (i&1) ? TC_YELLOW : TC_MAGENTA );
    window.move_to( 2, pos + 1 + i );
    window.puts( buffer );
  }

}

void Application::print_banner( TeleTextWindow &window ) {

  char buffer[41]; 

  struct tm now;
  time_t now_time;

  window.move_to( 0, 24 );
  window.set_color( TC_BLUE );
  window.set_bg( TC_BLACK );
  window.puts( "----------------------------------------" );

  time(&now_time);
  localtime_r( &now_time, &now );

  snprintf( buffer, 41, 
      "P500  TextSpace   %02d-%3s-%02d     %02d:%02d:%02d",
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

  // 10
  window.set_color( TC_WHITE );
  window.move_to( 0, 24 );

  //                                        ----------
  window.set_bg( TC_RED );    window.puts( "   News   ");
  window.set_bg( TC_YELLOW ); window.puts( "  Weather ");
  window.set_bg( TC_GREEN );  window.puts( "  Sports  ");
  window.set_bg( TC_BLUE );   window.puts( "   IRC    ");
}

int Application::main() {

  TeleTextWindow window;

  cout << "Bus timetable 2.0" << endl;

  if( !window.open() ) {
    cerr << "error: could not open window" << endl;
    return -1;
  }
  
  pthread_t fetcher_thread;

  m_run_thread = true;
  int r = pthread_create( &fetcher_thread, NULL, application_fetcher_thread, this );
  if( r == -1 ) {
    cerr << "error: could not start fetcher thread" << endl;
    return -1;
  }

  while( window.is_active() ) {

    print_banner( window );

    print_btf(  3, window, m_btf1 );
    print_btf( 12, window, m_btf2 );

    window.tick();
  }

  m_run_thread = false;
  pthread_join( fetcher_thread, NULL );
  
  return 0;
}
