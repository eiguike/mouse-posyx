#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "keyboard.h"
#include "server.h"

#define USE_SSL

MOUSE * gMouse = NULL;
KEYBOARD * gKeyboard = NULL;

int main() {
  gMouse = InitializeMouseDevice ();
  gKeyboard = InitializeKeyboardDevice ();

#ifndef USE_SSL
  SERVER * Server = InitializeServer(8000);
#else
  SERVER * Server = InitializeServerSSL(8000);
#endif
  
  Server->Start(Server);
}
