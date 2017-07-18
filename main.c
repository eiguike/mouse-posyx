#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>

MOUSE * gMouse = NULL;

enum OperatingSystem {
  LINUX = 0,
  WINDOWS
};

typedef struct {
  double x;
  double y;
  void * this;

  VOID * (*SetCursorPos)();
  int (*GetCursorPos)(double x, double y);
} MOUSE;

void SetupMouse() {
  gMouse = (MOUSE*)malloc(sizeof(MOUSE)*1);
  gMouse->this = &gMouse;

#ifdef LINUX
  gMouse->SetCursorPos = 
#elif WINDOWS
  gMouse->SetCursorPos = 
#endif

  gMouse->GetCursorPos = GetCursorPos;
}


int main() {
  SetupMouse ();
}
