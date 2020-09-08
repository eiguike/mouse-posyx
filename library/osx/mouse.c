#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>

#include <stdio.h>

#include "server.h"
#include "mouse.h"
#include "logger.h"

typedef enum MouseClickType {
  MOUSE_CLICK_TYPE_LEFT_BUTTON = 1,
  MOUSE_CLICK_TYPE_RIGHT_BUTTON = 3
};

void sendEventOsx(MOUSE *this, CGEventType mouseEventTypeConstant);

void SetCurrentPositionApple (MOUSE * this, POSITION NewPosition) {
  Logger->Info("SetCurrentPositionApple begin...");

  this->GetCurrentPosition(this);

  this->Position.X -= NewPosition.X;
  this->Position.Y -= NewPosition.Y;

  sendEventOsx(this, kCGEventMouseMoved);

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

void ClickEventApple (MOUSE * this, enum MouseClickType Button) {
  Logger->Info("ClickEventApple Button %d", Button);

  CGEventType mouseEventTypeConstant =
    Button == MOUSE_CLICK_TYPE_LEFT_BUTTON ?
      kCGEventLeftMouseDown : kCGEventRightMouseDown;

  sendEventOsx(this, mouseEventTypeConstant);
}

void ReleaseClickEventApple (MOUSE * this, enum MouseClickType Button) {
  Logger->Info("ReleaseClickEventApple Button %d", Button);

  CGEventType mouseEventTypeConstant =
    Button == MOUSE_CLICK_TYPE_RIGHT_BUTTON ?
      kCGEventLeftMouseUp : kCGEventRightMouseUp;

  sendEventOsx(this, mouseEventTypeConstant);
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

void sendEventOsx(MOUSE *this, CGEventType mouseEventTypeConstant) {
  CGEventRef mouseEvent = CGEventCreateMouseEvent(
      NULL,
      mouseEventTypeConstant,
      CGPointMake(this->Position.X, this->Position.Y),
      kCGMouseButtonLeft
  );

  if (mouseEvent == NULL) {
    Logger->Info("MoveMouse is null");
    return;
  }

  CGEventPost(kCGHIDEventTap, mouseEvent);
  CFRelease(mouseEvent);
}
