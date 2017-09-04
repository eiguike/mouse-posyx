#include "keyboard.h"

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>


void TypeLetterLinux (KEYBOARD * this, char * Input) {

  return;
  printf("TypeLetterLinux begin\n");
  printf("Letter received: %s\n", Input);
  KeyCode Keycode = 0;

  Keycode = XKeysymToKeycode(this->Display, XStringToKeysym(Input));

  if (Keycode == NoSymbol) {
    printf("broke letter\n");
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

