#include "server.h"
#include "mouse.h"

#ifdef LINUX
#include <X11/Xlib.h>

void SetCurrentPositionLinux (MOUSE * this, POSITION NewPosition) {

  XSelectInput(this->Display, this->Window, KeyReleaseMask);
  XWarpPointer(this->Display, None, None, 0, 0, 0, 0, -1 * NewPosition.X, -1 * NewPosition.Y);
  XFlush(this->Display);
}

POSITION GetCurrentPositionLinux (MOUSE * this) {
  int rootX, rootY, winX, winY;
  unsigned int mask;
  WINDOW root, child;

  XQueryPointer(this->Display, this->Window, &root, &child,
                &rootX, &rootY, &winX, &winY, &mask);

  this->Position.startX = rootX;
  this->Position.startY = rootX;

  return this->Position;
}
#endif

#ifdef WINDOWS
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

#endif

MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

#ifdef LINUX
  Mouse->GetCurrentPosition = GetCurrentPositionLinux;
  Mouse->SetCurrentPosition = SetCurrentPositionLinux;

  Mouse->Display = XOpenDisplay(0);
  Mouse->Window = XRootWindow(Mouse->Display, 0);

  Mouse->GetCurrentPosition (Mouse);
#elif WINDOWS
  Mouse->GetCurrentPosition = GetCurrentPositionWindows;
  Mouse->SetCurrentPosition = SetCurrentPositionWindows;
#endif


FINISH:
  return Mouse;
}
