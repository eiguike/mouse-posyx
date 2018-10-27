#ifndef LOGGER_H
#define LOGGER_H

typedef struct LOGGER_STRUCTURE {
  void (*Dispose) ();
  void (*Info) (const char* Format, ...);
  void (*Debug) (const char* Format, ...);
  void (*Error) (const char* Function, int Line, const char* Format, ...);
  void (*Warning) (const char* Format, ...);
} LOGGER;

int Logger_New();

extern LOGGER* Logger;
#endif
