#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "mouse.h"
#include "keyboard.h"
#include "server.h"

#include "logger.h"

MOUSE * gMouse = NULL;
KEYBOARD * gKeyboard = NULL;
static SERVER * Server = NULL;

void StopSignalHandler(int Signal);

int main() {
  Logger_New();
  gMouse = InitializeMouseDevice ();
  gKeyboard = InitializeKeyboardDevice ();

#ifndef USE_SSL
  Server = InitializeServer(8000);
#else
  Server = InitializeServerSSL(8000);
#endif

  if (Server == NULL) {
    Logger->Error(__FUNCTION__, __LINE__, "Could not initialize Server data structure");
    goto FINISH;
  }

  signal(SIGINT, StopSignalHandler);
  while(!Server->IsStop) {
    Server->Start(Server);
  }

FINISH:
  Logger->Dispose();
  Server->Dispose(&Server);
  return 0;
}

void StopSignalHandler(int Signal) {
  Server->Stop(Server);
}
