#include "server.h"
#include "mouse.h"

void SetCurrentPositionApple (MOUSE * this, POSITION NewPosition) {
  printf("SetCurrentPositionApple begin...\n");
  return;
}

POSITION GetCurrentPositionApple (MOUSE * this) {
  printf("GetCurrentPositionApple begin...\n");
  printf("X: %d\nY: %d\n", this->Position.X, this->Position.Y);
  return this->Position;
}

void ClickEventApple (MOUSE * this, const int Button) {
  printf("ClickEventApple begin...\n");
}

void ReleaseClickEventApple (MOUSE * this, const int Button) {
  printf("ReleaseClickEventApple begin...\n");
}


MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

  Mouse->ClickEvent = ClickEventApple;
  Mouse->ReleaseClickEvent = ReleaseClickEventApple;
  Mouse->GetCurrentPosition = GetCurrentPositionApple;
  Mouse->SetCurrentPosition = SetCurrentPositionApple;

FINISH:
  return Mouse;
}
