
#set(CMAKE_VERBOSE_MAKEFILE ON)

# our exe name
project (bus-times-2)

cmake_minimum_required (VERSION 2.6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g")

find_package (SDL REQUIRED)
find_package (SDL_image REQUIRED )

find_package (CURL REQUIRED)

#ADD_SUBDIRECTORY(third-party/curlcpp)
#INCLUDE_DIRECTORIES(${CURLCPP_SOURCE_DIR}/include)

# source files. sure, there's a better way of doing this
set (SRC
  src/application.cc
  src/main.cc
  src/teletext-window.cc
  src/data-fetcher.cc
  src/bus-time-fetcher.cc

  src/third-party/json-cpp/jsoncpp.cpp
)


link_libraries(
  ${SDL_LIBRARY}
  ${SDL_IMAGE_LIBRARIES}
  ${CURL_LIBRARY}
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/third-party/json-cpp)

add_executable(bus-times-2 ${SRC} )

add_custom_command(TARGET bus-times-2 POST_BUILD 
  COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "removing stale data
  directory"
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "linking data directory"
)
