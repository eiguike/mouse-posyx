#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "server.h"

MOUSE * gMouse = NULL;

int main() {
  SERVER * Server = InitializeServer(8000);

  gMouse = InitializeMouseDevice ();
  Server->Start(Server);
}
