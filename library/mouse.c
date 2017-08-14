#include "../include/server.h"
#include "../include/mouse.h"
#include <X11/Xlib.h>


void SetCurrentPositionLinux (MOUSE * this, POSITION NewPosition) {
  printf("SetCurrentPositionWindows begin...\n");
  printf("X: %d\nY: %d\n",NewPosition.X, NewPosition.Y);

  XSelectInput(this->Display, this->Window, KeyReleaseMask);
  XWarpPointer(this->Display, None, this->Window, 0, 0, 0, 0, NewPosition.X, NewPosition.Y);
  XFlush(this->Display);

  this->Position = NewPosition;
}

POSITION GetCurrentPositionLinux (MOUSE * this) {
  printf("GetCurrentPositionWindows begin...\n");
  printf("X: %d\nY: %d\n", this->Position.X, this->Position.Y);

  return this->Position;
}

void SetCurrentPositionWindows (MOUSE * this, POSITION NewPosition) {
  printf("SetCurrentPositionWindows begin...\n");

  this->Position = NewPosition;
  return;
}

POSITION GetCurrentPositionWindows (MOUSE * this) {
  printf("GetCurrentPositionWindows begin...\n");
  printf("X: %d\nY: %d\n", this->Position.X, this->Position.Y);

  return this->Position;
}

MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

  Mouse->GetCurrentPosition = GetCurrentPositionLinux;
  Mouse->SetCurrentPosition = SetCurrentPositionLinux;

  Mouse->Display = XOpenDisplay(0);
  Mouse->Window = XRootWindow(Mouse->Display, 0);
/*#elif WINDOWS
  Mouse->GetCurrentPosition = GetCurrentPositionWindows;
  Mouse->SetCurrentPosition = SetCurrentPositionWindows;
#endifa
*/

FINISH:
  return Mouse;
}
