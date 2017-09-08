#include "keyboard.h"

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>

#define XK_KP_Space                      0xff80  /* Space */
#define XK_KP_Enter                      0xff8d  /* Enter */
#define XK_BackSpace                     0xff08  /* Back space, back char */
#define XK_space                         0x0020
#define XK_Shift_L                       0xffe1  /* Left shift */


int isCaptalized(char Input) {
  printf("Letter received to compare %c\n", Input);
  printf("Input >= A: %s\n", (Input >= 'A' ? "True" : "False"));
  printf("Input <= Z: %s\n", (Input <= 'Z' ? "True" : "False"));
  printf("Value %d A: %d Z: %d\n", Input, 'A', 'Z');

  if (Input >= 'A' && Input <= 'Z') {
    return 1;
  }

  return 0;
}

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
    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
    if (isCaptalized(Input[0])) {
      XTestFakeKeyEvent(this->Display, Keycode, True, NULL);
      XFlush(this->Display);
    }
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

  // Disabling shift
  XTestFakeKeyEvent(this->Display, XKeysymToKeycode(this->Display, XK_Shift_L), False, NULL);
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

