#include "server.h"
#include "mouse.h"

#include <Foundation/Foundation.h>
#include <stdio.h>

void hello_world(const char *msg)
{
  NSString * myString = @"Hello World";
  NSLog(@"%@", myString);
}

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
