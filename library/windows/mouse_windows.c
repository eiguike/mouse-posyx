#include "server.h"
#include "mouse.h"
#include <windows.h>

void SetCurrentPositionWindows (MOUSE * this, POSITION NewPosition) {
  printf("SetCurrentPositionWindows begin...\n");
  POINT Pos;

  GetCursorPos(&Pos);

  printf("Received: X: %d Y: %d\n", NewPosition.X, NewPosition.Y);
  printf("Current: X: %d Y: %d\n", Pos.x, Pos.y);

  Pos.x = Pos.x - NewPosition.X;
  Pos.y  = Pos.y - NewPosition.Y;

  printf("New: X: %d Y: %d\n", Pos.x, Pos.y);

  SetCursorPos(Pos.x, Pos.y);

  return;
}

POSITION GetCurrentPositionWindows (MOUSE * this) {
  printf("GetCurrentPositionWindows begin...\n");
  printf("X: %d\nY: %d\n", this->Position.X, this->Position.Y);

  return this->Position;
}

void ClickEventWindows (MOUSE * this, const int Button) {
  printf("ClickEventWindows begin...\n");

  INPUT Input={0};

  memset(&Input, 0, sizeof(Input));
  Input.type = INPUT_MOUSE;

  if (Button == 1) {
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  } else {
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
  }

  SendInput(1, &Input, sizeof(INPUT));
}

void ReleaseClickEventWindows (MOUSE * this, const int Button) {
  printf("ReleaseClickEventWindows begin...\n");
  INPUT Input={0};

  memset(&Input, 0, sizeof(Input));
  Input.type = INPUT_MOUSE;

  if (Button == 1) {
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  } else {
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
  }

  SendInput(1, &Input, sizeof(INPUT));
}


MOUSE * InitializeMouseDevice () {
  MOUSE * Mouse = (MOUSE*)malloc(sizeof(MOUSE));

  if (Mouse == NULL) {
    goto FINISH;
  }

  Mouse->ClickEvent = ClickEventWindows;
  Mouse->ReleaseClickEvent = ReleaseClickEventWindows;
  Mouse->GetCurrentPosition = GetCurrentPositionWindows;
  Mouse->SetCurrentPosition = SetCurrentPositionWindows;

FINISH:
  return Mouse;
}
