
#pragma once 

#include <string>
#include <vector>

class BusTimeFetcher {
public:

  typedef struct _S_BUS_TIME {
    std::string bus;
    std::string destination;
    std::string time;
  } T_BUS_TIME, *PT_BUS_TIME;

  BusTimeFetcher( int );

  bool fetch();
  int count();

  T_BUS_TIME & operator[](int);

private:

  std::vector<T_BUS_TIME> m_bus_times;

  std::string m_fetch_url;

};

