#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

LOGGER* Logger = NULL;

void Logger_Debug (const char* Format, ...) {
  printf("DEBUG: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (stdout, Format, arg);
  va_end (arg);

  printf("\n");
  return;
}

void Logger_Info (const char* Format, ...) {
  printf("INFO: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (stdout, Format, arg);
  va_end (arg);

  printf("\n");
  return;
}

void Logger_Error (const char* Format, ...) {
  printf("ERROR: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (stdout, Format, arg);
  va_end (arg);

  printf("\n");
}

void Logger_Warning (const char* Format, ...) {
  printf("WARNING: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (stdout, Format, arg);
  va_end (arg);

  printf("\n");
}

void Logger_Dispose() {
  if (Logger != NULL) {
    free(Logger);
    Logger = NULL;
  }
}

int Logger_New() {
  int Status = 0;

  if (Logger != NULL) {
    Logger->Warning("You are trying to allocate Logger Library again!");
    goto FINISH;
  }

  Logger = calloc(1, sizeof(LOGGER));
  if (Logger == NULL) {
    printf("ERROR: Allocate Logger data strucuture failed\n");
    Status = -1;
    goto FINISH;
  }

  Logger->Dispose = Logger_Dispose;
  Logger->Info = Logger_Info;
  Logger->Debug = Logger_Debug;
  Logger->Error = Logger_Error;
  Logger->Warning = Logger_Warning;

FINISH:
  if (Logger != NULL) Logger->Info("Logger ready");

  return Status;
}
