cmake_minimum_required(VERSION 3.0)

project(mouse-posyx C)

add_library(Server STATIC library/server.c)
add_library(ServerHTTP STATIC library/protocols/http_protocol.c)
#add_library(ServerHTTP STATIC library/protocols/http_protocol_old.c)
add_library(ServerMouse STATIC library/protocols/mouse_protocol.c)
add_library(ServerKeyboard STATIC library/protocols/keyboard_protocol.c)

include_directories("./include")

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX AND NOT APPLE)
  add_library(Keyboard STATIC library/linux/keyboard_linux.c)
  add_library(Mouse STATIC library/linux/mouse_linux.c)

  target_link_libraries(Mouse X11)
  target_link_libraries(Keyboard Xtst)
  target_link_libraries(Server websockets)

  add_definitions(-DLINUX)
  MESSAGE("Building for Linux")
endif()

if(WIN32)
  add_library(Keyboard STATIC library/windows/keyboard_windows.c)
  add_library(Mouse STATIC library/windows/mouse_windows.c)

  include_directories("./libwebsockets/build")
  include_directories("./libwebsockets/lib")
  include_directories("./libwebsockets/win32port/win32helpers")
  include_directories("C:/OpenSSL-Win32/include")

  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/build/bin/websockets.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY C:/OpenSSL-Win32/libeay32.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY C:/OpenSSL-Win32/libssl32.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})	 
  file(COPY C:/OpenSSL-Win32/ssleay32.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	 
  target_link_libraries(Server "${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/build/lib/*.lib")

  add_definitions(-DWINDOWS)
  MESSAGE("Building for Windows")
endif()

if(APPLE)
  SET(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -w")
  MESSAGE("Building for Mac OS X")
  add_library(Keyboard STATIC library/osx/keyboard_apple.c)
  add_library(Mouse STATIC library/osx/mouse_apple.m)

  target_link_libraries(Server websockets)

  add_definitions(-DAPPLE)
endif()

if(USE_SSL)
  add_definitions(-DUSE_SSL)
  add_definitions(-DLWS_OPENSSL_SUPPORT)
  MESSAGE("SSL IS ACTIVATED")
else()
  MESSAGE("SSL IS DEACTIVATED")
endif()

target_link_libraries(Server ServerHTTP)
target_link_libraries(Server ServerMouse)
target_link_libraries(Server ServerKeyboard)
target_link_libraries(mouse-posyx Server)
target_link_libraries(mouse-posyx Mouse)
target_link_libraries(mouse-posyx Keyboard)

install(TARGETS mouse-posyx DESTINATION /bin)


