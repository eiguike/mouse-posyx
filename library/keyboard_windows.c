#include "keyboard.h"
#include <windows.h>

#define NoSymbol 0

static LETTER_ACCENT_TO_LETTER AccentLettersToLetter[] = {
  {
    "áàãâä",
    0,
    0x41
  },
  {
    "ÁÀÃÂÄ",
    1,
    0x41
  },
  {
    "ćĉ",
    0,
    0x43
  },
  {
    "ĆĈ",
    1,
    0x43
  },
  {
    "éèẽêë",
    0,
    0x45
  },
  {
    "ÉÈẼÊË",
    1,
    0x45
  },
  {
    "ǵĝ",
    0,
    0x47
  },
  {
    "ǴĜ",
    1,
    0x47
  },
  {
    "ĥḧ",
    0,
    0x48
  },
  {
    "ĤḦ",
    1,
    0x48
  },
  {
    "íìĩîï",
    0,
    0x49
  },
  {
    "ÍÌĨÎÏ",
    1,
    0x49
  },

  {
    "ĵ",
    0,
    0x4a
  },
  {
    "Ĵ",
    1,
    0x4a
  },
  {
    "ḱ",
    0,
    0x4b
  },
  {
    "Ḱ",
    1,
    0x4b
  },
  {
    "ĺ",
    0,
    0x4c
  },
  {
    "Ĺ",
    1,
    0x4c
  },
  {
    "ḿ",
    0,
    0x4d
  },
  {
    "Ḿ",
    1,
    0x4d
  },
  {
    "ńǹñ",
    0,
    0x4e
  },
  {
    "ŃǸÑ",
    1,
    0x4e
  },
  {
    "óòõôö",
    0,
    0x4f
  },
  {
    "ÓÒÕÔÖ",
    1,
    0x4f
  },
  {
    "ṕ",
    0,
    0x50
  },
  {
    "Ṕ",
    1,
    0x50
  },

  {
    "ŕ",
    0,
    0x52
  },
  {
    "Ŕ",
    1,
    0x52
  },
  {
    "śŝ",
    0,
    0x53
  },
  {
    "ŚŜ",
    1,
    0x53
  },
  {
    "úùũûü",
    0,
    0x55
  },
  {
    "ÚÙŨÛÜ",
    1,
    0x55
  },
  {
    "ǘǜṽ",
    0,
    0x56
  },
  {
    "ǗǛṼ",
    1,
    0x56
  },
  {
    "ẍ",
    0,
    0x57
  },
  {
    "Ẍ",
    1,
    0x57
  },
  {
    "ẃẁŵẅ",
    0,
    0x58
  },
  {
    "ẂẀŴẄ",
    1,
    0x58
  },
  {
    "ỳỹŷÿ",
    0,
    0x59
  },
  {
    "ỲỸŶŸ",
    1,
    0x59
  },
  {
    "źẑ",
    0,
    0x5a
  },
  {
    "ŹẐ",
    1,
    0x5a
  },
  {
    NULL,
    0,
    NoSymbol
  }
};

static LETTER_ACCENT AccentLetters[] = {
  {
    "ẃéŕýúíóṕáśǵḱĺḉźćǘńḿẂÉŔÝÚÍÓṔÁŚǴḰĹḈŹĆǗŃḾ",
    ACUTE,
    0,
    1,
    0xDB,
    "ACUTE"
  },
  {
    "çÇ",
    CEDILLA,
    0,
    0,
    0xBA,
    "CEDILLA"
  },
  {
    "ŴÊŶÛÎÔÂŜĜĤĴẐĈŵêŷûîôâŝĝĥĵẑĉ",
    CIRCUMFLEX,
    1,
    1,
    0xDE,
    "CIRCUMFLEX"
  },
  {
    "ẅëẗÿüïöäḧẍẄËŸÜÏÖÄḦẌ",
    DIAERESIS,
    1,
    1,
    0x36,
    "DIAERESIS"
  },
  {
    "ẁèỳùìòàǜǹẀÈỲÙÌÒÀǛǸ",
    GRAVE,
    1,
    1,
    0xDB,
    "GRAVE"
  },
  {
    "ẽỹũĩõãṽñẼỸŨĨÕÃṼÑ",
    TILDE,
    0,
    1,
    0xDE,
    "TILDE"
  },
  {
    NULL,
    NOTHING,
    0,
    0,
    NoSymbol,
    "NOTHING"
  }
};

//KeySym GetLetterFromAccent (char * Input, unsigned int * Return) {
//  unsigned int Index = 0;
//
//  while (AccentLettersToLetter[Index].AccentLetters != NULL) {
//    if (strstr(AccentLettersToLetter[Index].AccentLetters, Input) != NULL) {
//      printf("%s\n", AccentLettersToLetter[Index].AccentLetters);
//      *Return = Index;
//      return AccentLettersToLetter[Index].Letter;
//    }
//    Index++;
//  }
//  //printf("return NULL\n");
//  *Return = Index;
//  return AccentLettersToLetter[Index].Letter;
//}
//
//ACCENT GetAccent (char * Input) {
//  unsigned int Index = 0;
//
//  printf("GetAccent Input = %s\n", Input);
//
//  while(AccentLetters[Index].Letters != NULL) {
//    printf("%s %s\n", AccentLetters[Index].Letters, Input);
//    if (strstr(AccentLetters[Index].Letters, Input) != NULL) {
//      printf("Accent returned: %d\n", AccentLetters[Index].Accent);
//      return AccentLetters[Index].Accent;
//    }
//    Index++;
//  }
//
//  return AccentLetters[Index].Accent;
//}
//
//void TypeComplicatedLetter (KEYBOARD * this, char * Input) {
//  unsigned int Index;
//  KeyCode Keycode;
//  ACCENT Acc = GetAccent(Input);
//  printf("Accent: %s\n", AccentLetters[Acc].Type);
//
//  if (Acc == NOTHING) {
//    printf("I dont know how to interpret this...\n");
//    return;
//  }
//
//  if (AccentLetters[Acc].Capitalized) {
//    printf("Accent capitalized, pressing shift...\n");
//    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
//    XTestFakeKeyEvent(this->Display, Keycode, True, CurrentTime);
//  }
//
//  Keycode = XKeysymToKeycode(this->Display, AccentLetters[Acc].Keysym);
//  printf("Pressing keycode: %d\n", Keycode);
//  XTestFakeKeyEvent(this->Display, Keycode, True, CurrentTime);
//
//  Keycode = XKeysymToKeycode(this->Display, AccentLetters[Acc].Keysym);
//  printf("Releasing keycode: %d\n", Keycode);
//  XTestFakeKeyEvent(this->Display, Keycode, False, CurrentTime);
//
//  if (AccentLetters[Acc].Capitalized) {
//    printf("Releasing Shift\n");
//    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
//    XTestFakeKeyEvent(this->Display, Keycode, False, CurrentTime);
//    XFlush(this->Display);
//  }
//
//  if (AccentLetters[Acc].Composed) {
//    printf("Entrei aqui...\n");
//    KeySym Keysym = GetLetterFromAccent (Input, &Index);
//    Keycode = XKeysymToKeycode(this->Display, Keysym);
//    if (Keycode != NoSymbol) {
//      if (AccentLettersToLetter[Index].Capitalized) {
//        printf("Accent capitalized, pressing shift...\n");
//        XTestFakeKeyEvent(this->Display, XKeysymToKeycode (this->Display, XK_Shift_L), True, CurrentTime);
//      }
//
//      printf("Pressing Letter: %d\n", Keycode);
//      XTestFakeKeyEvent(this->Display, Keycode, True, 0);
//      XTestFakeKeyEvent(this->Display, Keycode, False, 0);
//      printf("Releasing Letter: %d\n", Keycode);
//
//      if (AccentLettersToLetter[Index].Capitalized) {
//        printf("Accent capitalized, releasing shift...\n");
//        XTestFakeKeyEvent(this->Display, XKeysymToKeycode (this->Display, XK_Shift_L), False, CurrentTime);
//      }
//    } else {
//      printf("I don't know how to handle this character %s\n", Input);
//    }
//  }
//
//  XFlush(this->Display);
//  printf("Finish...\n");
//  return;
//}

int IsLetterCapitalized(char * Input) {
  if (strlen(Input) == 1) {
    printf("Letter received to compare %c\n", Input[0]);
    printf("Input[0] >= A: %s\n", (Input[0] >= 'A' ? "True" : "False"));
    printf("Input[0] <= Z: %s\n", (Input[0] <= 'Z' ? "True" : "False"));
    printf("Value %d A: %d Z: %d\n", Input[0], 'A', 'Z');

    if (Input[0] >= 'A' && Input[0] <= 'Z') return 1;
    if (Input[0] >= '!' && Input[0] <= '+') {
      // treating special case in pt-br keyboard
      if (Input[0] == '\'') return 0;
      return 1;
    }
    if (Input[0] >= '>' && Input[0] <= '@') return 1;
    if (Input[0] >= '{' && Input[0] <= '~') return 1;
    if (Input[0] == '^') return 1;
    if (Input[0] == ':') return 1;
  } else {
    // another enconding
    char * CapitalizedWithAccent = "ẂẀÉÈẼÊËŔÝỲỸŸÚÙŨÛÜÍÌĨÎÏÓÒÕÔÖṔÃÂÁÀÄŜŚǴĜĤḦĴḰĹḈŹẐẌĆĈǗǛṼŃǸÑḾ"; // found on my keyboards heh
    if (strstr(CapitalizedWithAccent, Input) != NULL) {
      return 1;
    }
  }
  return 0;
}

/*
KeySym LatinStringToKeysym(char * Input) {

  // basic set of characters
  if (Input[0] >= 33 && Input[0] <= 126) {
    printf("Input 0x%x\n", Input[0]);
    return XK_exclam + (Input[0] - 33);
  }

  return XStringToKeysym(Input);
}*/


void TypeLetterWindows (KEYBOARD * this, char * Input) {
  printf("TypeLetterWindows begin\n");
  printf("Letter received: %s\n", Input);

  INPUT InputCommand;

  memset(&InputCommand, 0, sizeof(InputCommand));
  InputCommand.type = INPUT_KEYBOARD;

  if (strcmp(Input, "Backspace") == 0) {
    InputCommand.ki.wVk = 0x08;
  } else if (strcmp(Input, " ") == 0) {
    InputCommand.ki.wVk = 0x20;
  } else if (strcmp(Input, "Enter") == 0) {
    InputCommand.ki.wVk = 0x0D;
  } else {
  	printf("Valor: %d\n", Input[0]);

    if (IsLetterCapitalized (Input)) {
      printf("Capitalized!!\n");
      InputCommand.ki.wVk = 0x10; //shift
      SendInput(1, &InputCommand, sizeof(INPUT));
    }
    InputCommand.ki.wVk = 0x41 + Input[0];
  }

  //pressing button
  SendInput(1, &InputCommand, sizeof(INPUT));

  // release shift
  InputCommand.ki.wVk = 0x10; //shift
  InputCommand.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &InputCommand, sizeof(INPUT));
  
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

