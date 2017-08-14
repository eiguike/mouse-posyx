#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

typedef struct {
  unsigned int X;
  unsigned int Y;
} POSITION;

typedef struct MOUSE_DEVICE {
  POSITION Position;

  Display * Disp;
  Window Window;

  void (*GetCurrentPosition)(struct MOUSE_DEVICE * this);
  POSITION (*SetCurrentPosition)(struct MOUSE_DEVICE * this, POSITION Position);
} MOUSE;

MOUSE * InitializeMouseDevice ();
#endif
