cmake_minimum_required(VERSION 3.0)

project(mouse-posyx C)

add_library(Server STATIC library/server.c)
add_library(ServerHTTP STATIC library/protocols/http/http_protocol.c)
#add_library(ServerHTTP STATIC library/protocols/http_protocol_old.c)
add_library(ServerMouse STATIC library/protocols/mouse/mouse_protocol.c)
add_library(ServerKeyboard STATIC library/protocols/keyboard/keyboard_protocol.c)
add_library(ServerAbout STATIC library/protocols/about/about_protocol.c)
add_library(Logger STATIC library/logger.c)

include_directories("./include")

if(APPLE)
  link_directories("/usr/local/lib")
  include_directories("/usr/local/include")
endif()

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX AND NOT APPLE)
  add_library(Keyboard STATIC library/linux/keyboard.c)
  add_library(Mouse STATIC library/linux/mouse.c)

  target_link_libraries(Mouse X11)
  target_link_libraries(Keyboard Xtst)
  target_link_libraries(Server websockets)

  add_definitions(-DLINUX)
  MESSAGE("Building for Linux")
endif()

if(WIN32)
  add_library(Keyboard STATIC library/windows/keyboard.c)
  add_library(Mouse STATIC library/windows/mouse.c)

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
  SET(CMAKE_EXE_LINKER_FLAGS "-framework ApplicationServices -w")
  MESSAGE("Building for Mac OS X")
  add_library(Keyboard STATIC library/osx/keyboard.c)
  add_library(Mouse STATIC library/osx/mouse.c)

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
target_link_libraries(Server ServerAbout)
target_link_libraries(mouse-posyx Server)
target_link_libraries(mouse-posyx Mouse)
target_link_libraries(mouse-posyx Keyboard)
target_link_libraries(mouse-posyx Logger)

install(TARGETS mouse-posyx DESTINATION /bin)


