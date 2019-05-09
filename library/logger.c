#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

LOGGER* Logger = NULL;

static OUTPUT pOutput = STDOUT;
static FILE* pFileBuffer = NULL;

void Logger_Debug (const char* Format, ...) {
  fprintf(pFileBuffer, "DEBUG: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (pFileBuffer, Format, arg);
  va_end (arg);

  fprintf(pFileBuffer, "\n");
  return;
}

void Logger_Info (const char* Format, ...) {
  fprintf(pFileBuffer, "INFO: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (pFileBuffer, Format, arg);
  va_end (arg);

  fprintf(pFileBuffer, "\n");
  return;
}

void Logger_Error (const char* Function, int Line, const char* Format, ...) {
  fprintf(pFileBuffer, "ERROR %s.c:%d: ", Function, Line);

  va_list arg;
  va_start (arg, Format);
  vfprintf (pFileBuffer, Format, arg);
  va_end (arg);

  fprintf(pFileBuffer, "\n");
}

void Logger_Warning (const char* Format, ...) {
  fprintf(pFileBuffer, "WARNING: ");

  va_list arg;
  va_start (arg, Format);
  vfprintf (pFileBuffer, Format, arg);
  va_end (arg);

  fprintf(pFileBuffer, "\n");
}

void Logger_Dispose() {
  if (Logger != NULL) {
    free(Logger);
    Logger = NULL;

    if (pOutput == BINARY_FILE) {
      fclose(pFileBuffer);
      pFileBuffer = NULL;
    }
  }
}

int Logger_New(OUTPUT Output, ...) {
  int Status = 0;

  if (Logger != NULL) {
    Logger->Warning("You are trying to allocate Logger Library again!");
    goto FINISH;
  }

  if (Output == BINARY_FILE) {
    va_list arg;
    va_start (arg, Output);
    pFileBuffer = fopen(va_arg(arg, char*), "a+");
    va_end (arg);
  } else {
    pFileBuffer = stdout;
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
