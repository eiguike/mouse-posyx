#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX
#include <X11/Xlib.h>

typedef Display DISPLAY;
typedef Window WINDOW;

#endif

typedef struct {
  int X;
  int Y;
} POSITION;

typedef struct MOUSE_DEVICE {
  // Mouse position
  POSITION Position;

#ifdef LINUX
  // X11 Datastructure Related
  DISPLAY * Display;
  WINDOW Window;
#endif

  void (*ClickEvent)(struct MOUSE_DEVICE * this, const int ButtonValue);
  void (*ReleaseClickEvent)(struct MOUSE_DEVICE * this, const int ButtonValue);
  void (*GetCurrentPosition)(struct MOUSE_DEVICE * this);
  POSITION (*SetCurrentPosition)(struct MOUSE_DEVICE * this, POSITION Position);
} MOUSE;

MOUSE * InitializeMouseDevice ();
#endif
