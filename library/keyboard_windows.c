#include "keyboard.h"

void TypeLetterWindows (KEYBOARD * this, char * Input) {
  printf("TypeLetterWindows begin\n");
  printf("Letter received: %s\n", Input);
  printf("TypeLetterWindows end\n");
}

void ReleaseKeyboardWindows (KEYBOARD * this) {
  free(this);
}

KEYBOARD * InitializeKeyboardDevice() {
  KEYBOARD * Keyboard = (KEYBOARD*)malloc(sizeof(KEYBOARD));

  Keyboard->TypeLetter = TypeLetterWindows;
  Keyboard->Release = ReleaseKeyboardWindows;

  return Keyboard;
}

