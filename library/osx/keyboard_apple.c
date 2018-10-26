#include <string.h>

#include "logger.h"
#include "keyboard.h"

void TypeLetterApple (KEYBOARD * this, char * Input) {
  Logger->Info("TypeLetterApple begin");
  Logger->Info("Letter received: %s", Input);
  Logger->Info("TypeLetterApple end");
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

