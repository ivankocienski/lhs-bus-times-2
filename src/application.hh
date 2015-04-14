
#pragma once


#include "teletext-window.hh"
#include "bus-time-fetcher.hh"

class Application {
private:

  BusTimeFetcher m_btf1;
  BusTimeFetcher m_btf2;

  bool m_run_thread;

  friend void* application_fetcher_thread( void* );

  void print_btf( int, TeleTextWindow&, BusTimeFetcher& );
  
  void print_banner( TeleTextWindow& );
  
public:

  Application( int, char** );
  ~Application();

  int main();

};

