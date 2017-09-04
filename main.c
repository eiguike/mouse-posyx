#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "keyboard.h"
#include "server.h"

MOUSE * gMouse = NULL;
KEYBOARD * gKeyboard = NULL;

int main() {
  gMouse = InitializeMouseDevice ();
  gKeyboard = InitializeKeyboardDevice ();
  SERVER * Server = InitializeServer(8000);
  
  Server->Start(Server);
}
