#include <stdio.h>
#include <stdlib.h>

#define WINDOWS

typedef struct {
  double x;
  double y;
} MOUSE_POS;

typedef struct {
  MOUSE_POS pos;
} MOUSE;

MOUSE_POS GetMousePosition () {
  MOUSE_POS ret;
  return ret;
}

int main() {
  printf("Hello world!\n");
}
