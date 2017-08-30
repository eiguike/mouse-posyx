#include "server.h"
#include "mouse.h"

#ifdef LINUX
#include <X11/Xlib.h>
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
  XEvent Event;

  memset(&Event, 0, sizeof(Event));

  switch(ButtonValue) {
    default:
    case 1:
      Event.xbutton.button = Button1; // macro Button1 = Mouse1
      break;
    case 2:
      Event.xbutton.button = Button2; // macro Button1 = Mouse1
      break;
    case 3:
      Event.xbutton.button = Button3; // macro Button1 = Mouse1
      break;
  }
  Event.type = ButtonPress;
  Event.xbutton.same_screen = True;
  Event.xbutton.subwindow = DefaultRootWindow (this->Display);

  while (Event.xbutton.subwindow){
    Event.xbutton.window = Event.xbutton.subwindow;
    XQueryPointer (this->Display, Event.xbutton.window,
        &Event.xbutton.root, &Event.xbutton.subwindow,
        &Event.xbutton.x_root, &Event.xbutton.y_root,
        &Event.xbutton.x, &Event.xbutton.y,
        &Event.xbutton.state);
  }

  if(XSendEvent(this->Display, PointerWindow, True, ButtonPressMask, &Event) == 0) {
    printf("error1\n");
  }
  XFlush(this->Display);
  usleep(1);

  Event.type = ButtonRelease;
  if(XSendEvent(this->Display, PointerWindow, True, ButtonReleaseMask, &Event) == 0) {
    printf("error1\n");
  }
  XFlush(this->Display);
  usleep(1);
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

  if(Button == 1) {
    INPUT Input={0};

    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

    memset(&Input, 0, sizeof(Input));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
  } else {
    INPUT Input={0};

    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

    memset(&Input, 0, sizeof(Input));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &Input, sizeof(INPUT));
  }
}

#endif

MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

#ifdef LINUX
  Mouse->ClickEvent = ClickEventLinux;
  Mouse->GetCurrentPosition = GetCurrentPositionLinux;
  Mouse->SetCurrentPosition = SetCurrentPositionLinux;

  Mouse->Display = XOpenDisplay(0);
  Mouse->Window = XRootWindow(Mouse->Display, 0);

  Mouse->GetCurrentPosition (Mouse);
#elif WINDOWS
  Mouse->ClickEvent = ClickEventWindows;
  Mouse->GetCurrentPosition = GetCurrentPositionWindows;
  Mouse->SetCurrentPosition = SetCurrentPositionWindows;
#endif


FINISH:
  return Mouse;
}
