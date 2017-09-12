#include "keyboard.h"

#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

enum Accent {
  ACUTE,
  CEDILLA,
  CIRCUMFLEX,
  DIAERESIS,
  GRAVE,
  TILDE,
  NOTHING
};

typedef enum Accent ACCENT;

typedef struct LetterAccent {
  char * Letters;
  ACCENT Accent;
  int Capitalized;
  KeySym Keysym;

  char * Type;
} LETTER_ACCENT;

static LETTER_ACCENT Letters[] = {
  {
    "ẃéŕýúíóṕáśǵḱĺḉźćǘńḿẂÉŔÝÚÍÓṔÁŚǴḰĹḈŹĆǗŃḾ",
    ACUTE,
    0,
    XK_dead_acute,
    "ACUTE"
  },
  {
    "çÇ",
    CEDILLA,
    0,
    XK_ccedilla,
    "CEDILLA"
  },
  {
    "ŴÊŶÛÎÔÂŜĜĤĴẐĈŵêŷûîôâŝĝĥĵẑĉ",
    CIRCUMFLEX,
    1,
    XK_dead_circumflex,
    "CIRCUMFLEX"
  },
  {
    "ẅëẗÿüïöäḧẍẄËŸÜÏÖÄḦẌ",
    DIAERESIS,
    1,
    XK_dead_diaeresis,
    "DIAERESIS"
  },
  {
    "ẁèỳùìòàǜǹẀÈỲÙÌÒÀǛǸ",
    GRAVE,
    1,
    XK_dead_grave,
    "GRAVE"
  },
  {
    "ẽỹũĩõãṽñẼỸŨĨÕÃṼÑ",
    TILDE,
    0,
    XK_dead_tilde,
    "TILDE"
  },
  {
    NULL,
    NOTHING,
    0,
    NoSymbol,
    "NOTHING"
  }
};

ACCENT GetAccent (char * Input) {
  unsigned int Index = 0;

  //printf("GetAccent Input = %s\n", Input);

  while(Letters[Index].Letters != NULL) {
    //printf("%s %s\n", Letters[Index].Letters, Input);
    if (strstr(Letters[Index].Letters, Input) != NULL) {
      //printf("Accent returned: %d\n", Letters[Index].Accent);
      return Letters[Index].Accent;
    }
    Index++;
  }

  return Letters[Index].Accent;
}

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

KeySym LatinStringToKeysym(char * Input) {

  // basic set of characters
  if (Input[0] >= 33 && Input[0] <= 126) {
    printf("Input 0x%x\n", Input[0]);
    return XK_exclam + (Input[0] - 33);
  }

  return XStringToKeysym(Input);
}

void TypeLetterLinux (KEYBOARD * this, char * Input) {
  printf("TypeLetterLinux begin\n");
  printf("Letter received: %s\n", Input);

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
  } else {

    if (strlen(Input) == 1) {
      KeySym KeysymInput = LatinStringToKeysym(Input);

      if (KeysymInput == NoSymbol) {
        printf("Could not find any keysym available for this input: %s\n", Input);
        return;
      }

      Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
      if (IsLetterCapitalized(Input)) {
        printf("Shift pressed...\n");
        XTestFakeKeyEvent(this->Display, Keycode, True, 0);
        XFlush(this->Display);
      }
      Keycode = XKeysymToKeycode(this->Display, KeysymInput);
    } else {
      ACCENT Acc = GetAccent(Input);
      printf("Accent: %s\n", Letters[Acc].Type);

      if (Acc == NOTHING) {
        printf("I dont know how to interpret this...\n");
        return;
      }

      if (Letters[Acc].Capitalized) {
        printf("Pressed shift...\n");
        Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
        XTestFakeKeyEvent(this->Display, Keycode, True, 0);
        XFlush(this->Display);
      }

      Keycode = XKeysymToKeycode(this->Display, Letters[Acc].Keysym);
      printf("Pressed Keycode %d\n", Keycode);
      XTestFakeKeyEvent(this->Display, Keycode, True, 0);
      XFlush(this->Display);
      printf("Released Keycode %d\n", Keycode);
      XTestFakeKeyEvent(this->Display, Keycode, False, 0);
      XFlush(this->Display);

      Keycode = XKeysymToKeycode(this->Display, XK_Shift_L);
      XTestFakeKeyEvent(this->Display, Keycode, False, 0);
      XFlush(this->Display);

      // using another encode
      if (IsLetterCapitalized(Input)) {
        printf("Pressed A\n");

        Keycode = XKeysymToKeycode(this->Display, XK_A);
        XTestFakeKeyEvent(this->Display, Keycode, True, 0);
        XFlush(this->Display);

        printf("Released A\n");
        XTestFakeKeyEvent(this->Display, Keycode, False, 0);
        XFlush(this->Display);

        // Disabling shift
        XTestFakeKeyEvent(this->Display, XKeysymToKeycode(this->Display, XK_Shift_L), False, 0);
        XFlush(this->Display);
      } else {
        printf("Released shift...\n");
        // Disabling shift
        XTestFakeKeyEvent(this->Display, XKeysymToKeycode(this->Display, XK_Shift_L), False, 0);
        XFlush(this->Display);

        printf("Pressed A\n");

        Keycode = XKeysymToKeycode(this->Display, XK_A);
        XTestFakeKeyEvent(this->Display, Keycode, True, 0);
        XFlush(this->Display);

        printf("Released A\n");
        XTestFakeKeyEvent(this->Display, Keycode, False, 0);
        XFlush(this->Display);
      }

      printf("I don't know how to handle this kind of letter :(\n");
      return;
    }
  }

  if (Keycode == NoSymbol) {
    printf("Could not translate this Input: %s to a Keycode\n", Input);
    return;
  }

  XTestFakeKeyEvent(this->Display, Keycode, True, 0);
  XFlush(this->Display);
  XTestFakeKeyEvent(this->Display, Keycode, False, 0);
  XFlush(this->Display);

  // Disabling shift
  XTestFakeKeyEvent(this->Display, XKeysymToKeycode(this->Display, XK_Shift_L), False, 0);
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
  Keyboard->Window = XRootWindow(Keyboard->Display, 0);

  return Keyboard;
}

