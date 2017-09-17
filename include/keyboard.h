#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>


#ifdef LINUX
#include <X11/Xlib.h>
typedef Display DISPLAY;
typedef Window WINDOW;
#endif

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
  unsigned int Capitalized;
  unsigned int Composed;
  KeySym Keysym;
  char * Type;
} LETTER_ACCENT;

typedef struct LetterAccentToLetter {
  char * AccentLetters;
  unsigned int Capitalized;
  KeySym Letter;
} LETTER_ACCENT_TO_LETTER;

typedef struct KEYBOARD_DEVICE {

#ifdef LINUX
  DISPLAY * Display;
  WINDOW Window;
#endif

  void (*TypeLetter)(struct KEYBOARD_DEVICE * this, char * Input);
  void (*Release)(struct KEYBOARD_DEVICE * this);
} KEYBOARD;

KEYBOARD * InitializeKeyboardDevice();

#endif
