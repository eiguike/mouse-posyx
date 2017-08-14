#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include <X11/Xlib.h>

void SetCursorPos (int x, int y) {
  Display *dpy;
  Window root_window;

  dpy = XOpenDisplay(0);
  root_window = XRootWindow(dpy, 0);
  XSelectInput(dpy, root_window, KeyReleaseMask);
  XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
  XFlush(dpy);
}

int main() {
  SERVER * Server = InitializeServer(8000);
  Server->Start(Server);
}
