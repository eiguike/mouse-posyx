#include <Foundation/Foundation.h>
#include <stdio.h>

#include "server.h"
#include "mouse.h"
#include "logger.h"

void hello_world(const char *msg)
{
  NSString * myString = @"Hello World";
  NSLog(@"%@", myString);
}

void SetCurrentPositionApple (MOUSE * this, POSITION NewPosition) {
  Logger->Info("SetCurrentPositionApple begin...");
  return;
}

POSITION GetCurrentPositionApple (MOUSE * this) {
  Logger->Info("GetCurrentPositionApple begin...");
  Logger->Info("X: %d\nY: %d\n", this->Position.X, this->Position.Y);
  return this->Position;
}

void ClickEventApple (MOUSE * this, const int Button) {
  Logger->Info("ClickEventApple begin...");
}

void ReleaseClickEventApple (MOUSE * this, const int Button) {
  Logger->Info("ReleaseClickEventApple begin...");
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
