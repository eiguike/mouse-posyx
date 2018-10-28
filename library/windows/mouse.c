#include <windows.h>

#include "server.h"
#include "mouse.h"
#include "logger.h"

void SetCurrentPositionWindows (MOUSE * this, POSITION NewPosition) {
  Logger->Info("SetCurrentPositionWindows begin...");
  POINT Pos;

  GetCursorPos(&Pos);

  Logger->Info("Received: X: %d Y: %d", NewPosition.X, NewPosition.Y);
  Logger->Info("Current: X: %d Y: %d", Pos.x, Pos.y);

  Pos.x = Pos.x - NewPosition.X;
  Pos.y  = Pos.y - NewPosition.Y;

  Logger->Info("New: X: %d Y: %d", Pos.x, Pos.y);

  SetCursorPos(Pos.x, Pos.y);

  return;
}

POSITION GetCurrentPositionWindows (MOUSE * this) {
  Logger->Info("GetCurrentPositionWindows begin...");
  Logger->Info("X: %d\nY: %d", this->Position.X, this->Position.Y);

  return this->Position;
}

void ClickEventWindows (MOUSE * this, const int Button) {
  Logger->Info("ClickEventWindows begin...");

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
  Logger->Info("ReleaseClickEventWindows begin...");
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
