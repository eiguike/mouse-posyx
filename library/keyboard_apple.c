#include "keyboard.h"
#include <string.h>

void TypeLetterApple (KEYBOARD * this, char * Input) {
  printf("TypeLetterApple begin\n");
  printf("Letter received: %s\n", Input);
  printf("TypeLetterApple end\n");
}

void ReleaseKeyboardApple (KEYBOARD * this) {
  free(this);
}

KEYBOARD * InitializeKeyboardDevice() {
  KEYBOARD * Keyboard = (KEYBOARD*)malloc(sizeof(KEYBOARD));

  Keyboard->TypeLetter = TypeLetterApple;
  Keyboard->Release = ReleaseKeyboardApple;

  return Keyboard;
}

