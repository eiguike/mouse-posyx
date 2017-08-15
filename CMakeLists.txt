cmake_minimum_required(VERSION 3.0)

project(mouse-posyx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lX11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwebsockets")

add_library(Mouse STATIC library/mouse.c)
add_library(Server STATIC library/server.c)

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

target_link_libraries(Server websockets)

if(DEBUG)
  add_definitions(-DLINUX)
  MESSAGE("Building for LINUX")
  target_link_libraries(Mouse X11)
endif()

install(TARGETS mouse-posyx DESTINATION /bin)


