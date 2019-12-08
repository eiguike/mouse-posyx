#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>

#include <stdio.h>

#include "server.h"
#include "mouse.h"
#include "logger.h"

void SetCurrentPositionApple (MOUSE * this, POSITION NewPosition) {
  Logger->Info("SetCurrentPositionApple begin...");

  this->GetCurrentPosition(this);

  this->Position.X -= NewPosition.X;
  this->Position.Y -= NewPosition.Y;

  CGEventRef MoveMouse = CGEventCreateMouseEvent(NULL,
                                      kCGEventMouseMoved,
                                      CGPointMake(this->Position.X, this->Position.Y),
                                      kCGMouseButtonLeft);

  CGEventPost(kCGHIDEventTap, MoveMouse);


  CFRelease(MoveMouse);

  return;
}

POSITION GetCurrentPositionApple (MOUSE * this) {
  Logger->Info("GetCurrentPositionApple begin...");

  CGEventRef Event = CGEventCreate(nil);
  CGPoint Localization = CGEventGetLocation(Event);

  this->Position.X = Localization.x;
  this->Position.Y = Localization.y;

  CFRelease(Event);

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
