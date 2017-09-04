#include "keyboard.h"

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>

#define XK_KP_Space                      0xff80  /* Space */
#define XK_KP_Enter                      0xff8d  /* Enter */
#define XK_BackSpace                     0xff08  /* Back space, back char */
#define XK_space                         0x0020

void TypeLetterLinux (KEYBOARD * this, char * Input) {
  printf("TypeLetterLinux begin\n");
  printf("Letter received: %s\n", Input);
  KeyCode Keycode = 0;

  if (strcmp(Input, "Backspace") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_BackSpace);
  } else if (strcmp(Input, " ") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_space);
  } else if (strcmp(Input, "Enter") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_KP_Enter);
  } else {
    Keycode = XKeysymToKeycode(this->Display, XStringToKeysym(Input));
  }

  if (Keycode == NoSymbol) {
    printf("Could not translate this Input: %s to a Keycode\n", Input);
    return;
  }

  XTestFakeKeyEvent(this->Display, Keycode, True, NULL);
  XFlush(this->Display);
  XTestFakeKeyEvent(this->Display, Keycode, False, NULL);
  XFlush(this->Display);

  printf("TypeLetterLinux end\n");
}

void ReleaseKeyboardLinux (KEYBOARD * this) {
  free(this);
}

KEYBOARD * InitializeKeyboardDevice() {
  KEYBOARD * Keyboard = (KEYBOARD*)malloc(sizeof(KEYBOARD));

  Keyboard->TypeLetter = TypeLetterLinux;
  Keyboard->Release = ReleaseKeyboardLinux;
  Keyboard->Display = XOpenDisplay(NULL);
  Keyboard->Window = XRootWindow(Keyboard->Display, NULL);

  return Keyboard;
}

