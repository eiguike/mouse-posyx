#include "keyboard.h"
#include <windows.h>

void TypeLetterWindows (KEYBOARD * this, char * Input) {
  printf("TypeLetterWindows begin\n");
  printf("Letter received: %s\n", Input);

  INPUT InputCommand;

  memset(&InputCommand, 0, sizeof(InputCommand));
  InputCommand.type = INPUT_KEYBOARD;
  InputCommand.ki.wScan = 0;
  InputCommand.ki.time = 0;
  InputCommand.ki.dwExtraInfo = 0;
  InputCommand.ki.wVk = 0x41;
  InputCommand.mi.dwFlags = KEYEVENTF_SCANCODE;

  if (strcmp(Input, "Backspace") == 0) {
    InputCommand.ki.wVk = 0x08;
  } else if (strcmp(Input, " ") == 0) {
    InputCommand.ki.wVk = 0x20;
  } else if (strcmp(Input, "Enter") == 0) {
    InputCommand.ki.wVk = 0x0D;
  } else {
  	printf("Valor: %d\n", Input[0]);
    InputCommand.ki.wVk = 0x41 + (Input[0] - 65);
  }

  SendInput(1, &InputCommand, sizeof(INPUT));

  InputCommand.mi.dwFlags = KEYEVENTF_KEYUP;
  
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

