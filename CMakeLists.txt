cmake_minimum_required(VERSION 3.0)

project(mouse-posyx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_library(Mouse STATIC library/mouse.c)
add_library(Server STATIC library/server.c)

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(mouse-posyx Server)
target_link_libraries(mouse-posyx Mouse)

target_link_libraries(Server websockets)

if(UNIX)
  target_link_libraries(Mouse X11)
  add_definitions(-DLINUX)
  MESSAGE("Building for Linux")
endif()

if(WIN32)
  target_link_libraries(Mouse X11)
  add_definitions(-DWINDOWS)
  MESSAGE("Building for Windows")
endif()

install(TARGETS mouse-posyx DESTINATION /bin)


