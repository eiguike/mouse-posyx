#include "server.h"
#include "mouse.h"

#ifdef LINUX
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

#endif

#ifdef WINDOWS
#include <windows.h>

void SetCurrentPositionWindows (MOUSE * this, POSITION NewPosition) {
  printf("SetCurrentPositionWindows begin...\n");
  POINT Pos;

  GetCursorPos(&Pos);

  printf("Received: X: %d Y: %d\n", NewPosition.X, NewPosition.Y);
  printf("Current: X: %d Y: %d\n", Pos.x, Pos.y);

  Pos.x = Pos.x - NewPosition.X;
  Pos.y  = Pos.y - NewPosition.Y;

  printf("New: X: %d Y: %d\n", Pos.x, Pos.y);

  SetCursorPos(Pos.x, Pos.y);

  return;
}

POSITION GetCurrentPositionWindows (MOUSE * this) {
  printf("GetCurrentPositionWindows begin...\n");
  printf("X: %d\nY: %d\n", this->Position.X, this->Position.Y);

  return this->Position;
}

void ClickEventWindows (MOUSE * this, const int Button) {
  printf("ClickEventWindows begin...\n");

  INPUT Input={0};

  memset(&Input, 0, sizeof(Input));
  Input.type = INPUT_MOUSE;

  if (Button == 1) {
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  } else {
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
  }

  SendInput(1, &Input, sizeof(INPUT));
}

void ReleaseClickEventWindows (MOUSE * this, const int Button) {
  printf("ReleaseClickEventWindows begin...\n");
  INPUT Input={0};

  memset(&Input, 0, sizeof(Input));
  Input.type = INPUT_MOUSE;

  if (Button == 1) {
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  } else {
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
  }

  SendInput(1, &Input, sizeof(INPUT));
}


#endif

MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

#ifdef LINUX
  Mouse->ClickEvent = ClickEventLinux;
  Mouse->ReleaseClickEvent = ReleaseClickEventLinux;
  Mouse->GetCurrentPosition = GetCurrentPositionLinux;
  Mouse->SetCurrentPosition = SetCurrentPositionLinux;

  Mouse->Display = XOpenDisplay(0);
  Mouse->Window = XRootWindow(Mouse->Display, 0);

  Mouse->GetCurrentPosition (Mouse);
#elif WINDOWS
  Mouse->ClickEvent = ClickEventWindows;
  Mouse->ReleaseClickEvent = ReleaseClickEventWindows;
  Mouse->GetCurrentPosition = GetCurrentPositionWindows;
  Mouse->SetCurrentPosition = SetCurrentPositionWindows;
#endif


FINISH:
  return Mouse;
}
