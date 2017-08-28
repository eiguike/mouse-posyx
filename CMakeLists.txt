cmake_minimum_required(VERSION 3.0)

project(mouse-posyx C)

add_library(Mouse STATIC library/mouse.c)
add_library(Server STATIC library/server.c)
add_library(ServerMouse STATIC library/protocols/mouse_protocol.c)
add_library(ServerHTTP STATIC library/protocols/http_protocol.c)

include_directories("./include")

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Server ServerMouse)
target_link_libraries(Server ServerHTTP)
target_link_libraries(mouse-posyx Server)
target_link_libraries(mouse-posyx Mouse)

if(UNIX)
  target_link_libraries(Mouse X11)
  target_link_libraries(Server websockets)
  add_definitions(-DLINUX)
  MESSAGE("Building for Linux")
endif()

if(WIN32)
  include_directories("./libwebsockets/build/include")
  include_directories("C:/OpenSSL-Win32/include")

  target_link_libraries(Server "${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/build/lib/*.lib")

  add_definitions(-DWINDOWS)
  MESSAGE("Building for Windows")
endif()

install(TARGETS mouse-posyx DESTINATION /bin)


