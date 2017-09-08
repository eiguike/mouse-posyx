#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>


#ifdef LINUX
#include <X11/Xlib.h>
typedef Display DISPLAY;
typedef Window WINDOW;
#endif

typedef struct KEYBOARD_DEVICE {

#ifdef LINUX
  DISPLAY * Display;
  WINDOW * Window;
#endif

  void (*TypeLetter)(struct KEYBOARD_DEVICE * this, char * Input);
  void (*Release)(struct KEYBOARD_DEVICE * this);
} KEYBOARD;

KEYBOARD * InitializeKeyboardDevice();

#endif
