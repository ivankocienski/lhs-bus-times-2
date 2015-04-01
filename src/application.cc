
#include <iostream>

#include "application.hh"
#include "teletext-window.hh"

using namespace std;

Application::Application( int argc, char **argv ) {
}

Application::~Application() {
}

int Application::main() {

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
