#include "server.h"
#include "mouse.h"

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <string.h>

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

  this->Position.X = rootX;
  this->Position.Y = rootX;

  return this->Position;
}

void ClickEventLinux (MOUSE * this, const int ButtonValue) {
  printf("ClickEventLinux begin...\n");
  printf("ButtonValue = %d\n", ButtonValue);
  XTestFakeButtonEvent (this->Display, ButtonValue, 1, CurrentTime);
  XFlush(this->Display);
}

void ReleaseClickEventLinux (MOUSE * this, const int ButtonValue) {
  printf("ReleaseClickEventLinux begin...\n");
  printf("ButtonValue = %d\n", ButtonValue);
  XTestFakeButtonEvent (this->Display, ButtonValue, 0, CurrentTime);
  XFlush(this->Display);
}

MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

  Mouse->ClickEvent = ClickEventLinux;
  Mouse->ReleaseClickEvent = ReleaseClickEventLinux;
  Mouse->GetCurrentPosition = GetCurrentPositionLinux;
  Mouse->SetCurrentPosition = SetCurrentPositionLinux;

  Mouse->Display = XOpenDisplay(0);
  Mouse->Window = XRootWindow(Mouse->Display, 0);

  Mouse->GetCurrentPosition (Mouse);
FINISH:
  return Mouse;
}
