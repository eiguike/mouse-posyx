#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "server.h"

MOUSE * gMouse = NULL;

int main() {
  gMouse = InitializeMouseDevice ();
  SERVER * Server = InitializeServer(8000);
  
  Server->Start(Server);
}
