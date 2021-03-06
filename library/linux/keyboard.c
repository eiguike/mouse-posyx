#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

#include "keyboard.h"
#include "logger.h"


static LETTER_ACCENT_TO_LETTER AccentLettersToLetter[] = {
  {
    "áàãâä",
    0,
    XK_a
  },
  {
    "ÁÀÃÂÄ",
    1,
    XK_A
  },
  {
    "ćĉ",
    0,
    XK_c
  },
  {
    "ĆĈ",
    1,
    XK_C
  },
  {
    "éèẽêë",
    0,
    XK_e
  },
  {
    "ÉÈẼÊË",
    1,
    XK_E
  },
  {
    "ǵĝ",
    0,
    XK_g
  },
  {
    "ǴĜ",
    1,
    XK_G
  },
  {
    "ĥḧ",
    0,
    XK_h
  },
  {
    "ĤḦ",
    1,
    XK_H
  },
  {
    "íìĩîï",
    0,
    XK_i
  },
  {
    "ÍÌĨÎÏ",
    1,
    XK_i
  },

  {
    "ĵ",
    0,
    XK_j
  },
  {
    "Ĵ",
    1,
    XK_J
  },
  {
    "ḱ",
    0,
    XK_k
  },
  {
    "Ḱ",
    1,
    XK_K
  },
  {
    "ĺ",
    0,
    XK_l
  },
  {
    "Ĺ",
    1,
    XK_L
  },
  {
    "ḿ",
    0,
    XK_m
  },
  {
    "Ḿ",
    1,
    XK_M
  },
  {
    "ńǹñ",
    0,
    XK_n
  },
  {
    "ŃǸÑ",
    1,
    XK_N
  },
  {
    "óòõôö",
    0,
    XK_o
  },
  {
    "ÓÒÕÔÖ",
    1,
    XK_O
  },
  {
    "ṕ",
    0,
    XK_p
  },
  {
    "Ṕ",
    1,
    XK_P
  },

  {
    "ŕ",
    0,
    XK_r
  },
  {
    "Ŕ",
    1,
    XK_R
  },
  {
    "śŝ",
    0,
    XK_s
  },
  {
    "ŚŜ",
    1,
    XK_S
  },
  {
    "úùũûü",
    0,
    XK_u
  },
  {
    "ÚÙŨÛÜ",
    1,
    XK_U
  },
  {
    "ǘǜṽ",
    0,
    XK_v
  },
  {
    "ǗǛṼ",
    1,
    XK_V
  },
  {
    "ẍ",
    0,
    XK_x
  },
  {
    "Ẍ",
    1,
    XK_X
  },
  {
    "ẃẁŵẅ",
    0,
    XK_w
  },
  {
    "ẂẀŴẄ",
    1,
    XK_W
  },
  {
    "ỳỹŷÿ",
    0,
    XK_y
  },
  {
    "ỲỸŶŸ",
    1,
    XK_Y
  },
  {
    "źẑ",
    0,
    XK_z
  },
  {
    "ŹẐ",
    1,
    XK_Z
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
    XK_dead_acute,
    "ACUTE"
  },
  {
    "çÇ",
    CEDILLA,
    0,
    0,
    XK_ccedilla,
    "CEDILLA"
  },
  {
    "ŴÊŶÛÎÔÂŜĜĤĴẐĈŵêŷûîôâŝĝĥĵẑĉ",
    CIRCUMFLEX,
    1,
    1,
    XK_dead_circumflex,
    "CIRCUMFLEX"
  },
  {
    "ẅëẗÿüïöäḧẍẄËŸÜÏÖÄḦẌ",
    DIAERESIS,
    1,
    1,
    XK_dead_diaeresis,
    "DIAERESIS"
  },
  {
    "ẁèỳùìòàǜǹẀÈỲÙÌÒÀǛǸ",
    GRAVE,
    1,
    1,
    XK_dead_grave,
    "GRAVE"
  },
  {
    "ẽỹũĩõãṽñẼỸŨĨÕÃṼÑ",
    TILDE,
    0,
    1,
    XK_dead_tilde,
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

KeySym GetLetterFromAccent (char * Input, unsigned int * Return) {
  unsigned int Index = 0;

  while (AccentLettersToLetter[Index].AccentLetters != NULL) {
    if (strstr(AccentLettersToLetter[Index].AccentLetters, Input) != NULL) {
      Logger->Info("%s\n", AccentLettersToLetter[Index].AccentLetters);
      *Return = Index;
      return AccentLettersToLetter[Index].Letter;
    }
    Index++;
  }
  //Logger->Info("return NULL\n");
  *Return = Index;
  return AccentLettersToLetter[Index].Letter;
}

ACCENT GetAccent (char * Input) {
  unsigned int Index = 0;

  Logger->Info("GetAccent Input = %s\n", Input);

  while(AccentLetters[Index].Letters != NULL) {
    Logger->Info("%s %s\n", AccentLetters[Index].Letters, Input);
    if (strstr(AccentLetters[Index].Letters, Input) != NULL) {
      Logger->Info("Accent returned: %d\n", AccentLetters[Index].Accent);
      return AccentLetters[Index].Accent;
    }
    Index++;
  }

  return AccentLetters[Index].Accent;
}

void TypeComplicatedLetter (KEYBOARD * this, char * Input) {
  unsigned int Index;
  KeyCode Keycode;
  ACCENT Acc = GetAccent(Input);
  Logger->Info("Accent: %s\n", AccentLetters[Acc].Type);

  if (Acc == NOTHING) {
    Logger->Info("I dont know how to interpret this...\n");
    return;
  }

  if (AccentLetters[Acc].Capitalized) {
    Logger->Info("Accent capitalized, pressing shift...\n");
    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
    XTestFakeKeyEvent(this->Display, Keycode, True, CurrentTime);
  }

  Keycode = XKeysymToKeycode(this->Display, AccentLetters[Acc].Keysym);
  Logger->Info("Pressing keycode: %d\n", Keycode);
  XTestFakeKeyEvent(this->Display, Keycode, True, CurrentTime);

  Keycode = XKeysymToKeycode(this->Display, AccentLetters[Acc].Keysym);
  Logger->Info("Releasing keycode: %d\n", Keycode);
  XTestFakeKeyEvent(this->Display, Keycode, False, CurrentTime);

  if (AccentLetters[Acc].Capitalized) {
    Logger->Info("Releasing Shift\n");
    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
    XTestFakeKeyEvent(this->Display, Keycode, False, CurrentTime);
    XFlush(this->Display);
  }

  if (AccentLetters[Acc].Composed) {
    Logger->Info("AccentLetters Composed == TRUE\n");
    KeySym Keysym = GetLetterFromAccent (Input, &Index);
    Keycode = XKeysymToKeycode(this->Display, Keysym);
    if (Keycode != NoSymbol) {
      if (AccentLettersToLetter[Index].Capitalized) {
        Logger->Info("Accent capitalized, pressing shift...\n");
        XTestFakeKeyEvent(this->Display, XKeysymToKeycode (this->Display, XK_Shift_L), True, CurrentTime);
      }

      Logger->Info("Pressing Letter: %d\n", Keycode);
      XTestFakeKeyEvent(this->Display, Keycode, True, 0);
      XTestFakeKeyEvent(this->Display, Keycode, False, 0);
      Logger->Info("Releasing Letter: %d\n", Keycode);

      if (AccentLettersToLetter[Index].Capitalized) {
        Logger->Info("Accent capitalized, releasing shift...\n");
        XTestFakeKeyEvent(this->Display, XKeysymToKeycode (this->Display, XK_Shift_L), False, CurrentTime);
      }
    } else {
      Logger->Info("I don't know how to handle this character %s\n", Input);
    }
  }

  XFlush(this->Display);
  Logger->Info("Finish...\n");
  return;
}

int IsLetterCapitalized(char * Input) {
  if (strlen(Input) == 1) {
    Logger->Info("Letter received to compare %c\n", Input[0]);
    Logger->Info("Input[0] >= A: %s\n", (Input[0] >= 'A' ? "True" : "False"));
    Logger->Info("Input[0] <= Z: %s\n", (Input[0] <= 'Z' ? "True" : "False"));
    Logger->Info("Value %d A: %d Z: %d\n", Input[0], 'A', 'Z');

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

KeySym LatinStringToKeysym(char * Input) {

  // basic set of characters
  if (Input[0] >= 33 && Input[0] <= 126) {
    Logger->Info("Input 0x%x\n", Input[0]);
    return XK_exclam + (Input[0] - 33);
  }

  return XStringToKeysym(Input);
}

void TypeLetterLinux (KEYBOARD * this, char * Input) {
  Logger->Info("TypeLetterLinux begin\n");
  Logger->Info("Letter received: %s\n", Input);

  // special treatment when @ is sent
  if (Input == NULL) {
    char At = '@';
    Input = &At;
  }

  KeyCode Keycode = 0;

  if (strcmp(Input, "Backspace") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_BackSpace);
  } else if (strcmp(Input, " ") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_space);
  } else if (strcmp(Input, "\n") == 0) {
    Keycode = XKeysymToKeycode(this->Display, XK_KP_Enter);
  } else if (strlen(Input) > 1) {
    TypeComplicatedLetter (this, Input);
    return;
  } else if (strlen(Input) == 1) {
    KeySym KeysymInput = LatinStringToKeysym(Input);

    if (KeysymInput == NoSymbol) {
      Logger->Info("Could not find any keysym available for this input: %s\n", Input);
      return;
    }

    Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
    if (IsLetterCapitalized(Input)) {
      Logger->Info("Shift pressed...\n");
      XTestFakeKeyEvent(this->Display, Keycode, True, 0);
      XFlush(this->Display);
    }
    Keycode = XKeysymToKeycode(this->Display, KeysymInput);
  }

  if (Keycode == NoSymbol) {
    Logger->Info("Could not translate this Input: %s to a Keycode\n", Input);
    return;
  }

  XTestFakeKeyEvent(this->Display, Keycode, True, 0);
  XFlush(this->Display);
  XTestFakeKeyEvent(this->Display, Keycode, False, 0);
  XFlush(this->Display);

  // Disabling shift
  XTestFakeKeyEvent(this->Display, XKeysymToKeycode(this->Display, XK_Shift_L), False, 0);
  XFlush(this->Display);

  Logger->Info("TypeLetterLinux end\n");
}

void ReleaseKeyboardLinux (KEYBOARD * this) {
  free(this);
}

KEYBOARD * InitializeKeyboardDevice() {
  KEYBOARD * Keyboard = (KEYBOARD*)malloc(sizeof(KEYBOARD));

  ///////////////////////////////////////////////////
  // little fix to set to Xlib the correct layout //
  //////////////////////////////////////////////////
  FILE *ListLayout = popen("setxkbmap -query | grep layout", "r");
  char Buffer[256];
  char Layout[10];

  while (fgets(Buffer, sizeof(Buffer), ListLayout) != 0) {
    strtok(Buffer, "     ");
    strcpy(Layout, strtok(NULL, "     "));
  }
  pclose(ListLayout);
  Logger->Info("Keyboard layout: %s", Layout);

  memset(Buffer, 0, 256);
  strcat(Buffer, "setxkbmap ");
  strcat(Buffer, Layout);
  system(Buffer);
  Logger->Info("Setting Keyboard Layout to: %s\n", Layout);
  // https://bugs.launchpad.net/ubuntu/+source/xorg-server/+bug/837456
  ///////////////////////////////////////////////////
  ///////////////////////////////////////////////////

  Keyboard->TypeLetter = TypeLetterLinux;
  Keyboard->Release = ReleaseKeyboardLinux;
  Keyboard->Display = XOpenDisplay(NULL);
  Keyboard->Window = XRootWindow(Keyboard->Display, 0);

  return Keyboard;
}

