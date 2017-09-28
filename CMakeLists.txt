cmake_minimum_required(VERSION 3.0)

project(mouse-posyx C)

add_library(Server STATIC library/server.c)
add_library(ServerMouse STATIC library/protocols/mouse_protocol.c)
add_library(ServerKeyboard STATIC library/protocols/keyboard_protocol.c)

include_directories("./include")

set(SOURCE_FILES main.c)
add_executable(mouse-posyx ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/index.html
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX)
  add_library(Keyboard STATIC library/keyboard_linux.c)
  add_library(Mouse STATIC library/mouse_linux.c)

  target_link_libraries(Mouse X11)
  target_link_libraries(Keyboard Xtst)
  target_link_libraries(Server websockets)

  add_definitions(-DLINUX)
  MESSAGE("Building for Linux")
endif()

if(WIN32)
  add_library(Keyboard STATIC library/keyboard_windows.c)
  add_library(Mouse STATIC library/mouse_windows.c)

  include_directories("./libwebsockets/build/include")
  include_directories("C:/OpenSSL-Win32/include")

  target_link_libraries(Server "${CMAKE_CURRENT_SOURCE_DIR}/libwebsockets/build/lib/Release/*.lib")

  add_definitions(-DWINDOWS)
  MESSAGE("Building for Windows")
endif()

if(USE_SSL)
  add_definitions(-DUSE_SSL)
  add_definitions(-DLWS_OPENSSL_SUPPORT)
  MESSAGE("SSL IS ACTIVATED")
else()
  MESSAGE("SSL IS DEACTIVATED")
endif()

target_link_libraries(Server ServerMouse)
target_link_libraries(Server ServerKeyboard)
target_link_libraries(mouse-posyx Server)
target_link_libraries(mouse-posyx Mouse)
target_link_libraries(mouse-posyx Keyboard)

install(TARGETS mouse-posyx DESTINATION /bin)


