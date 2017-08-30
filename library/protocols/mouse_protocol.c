#include "server.h"
#include "mouse.h"

#include <string.h>

extern MOUSE * gMouse;

static COMMAND_MAPPING CommandMapping[] = {
  {
    "MOVE",
    PositionParser
  },
  {
    "RELEASE_RIGHT",
    ReleaseClickParser
  },
  {
    "RELEASE_LEFT",
    ReleaseClickParser
  },
  {
    "RIGHT_CLICK",
    ClickParser
  },
  {
    "LEFT_CLICK",
    ClickParser
  },
  {
    NULL, NULL
  }
};

void PositionParser (char * Input) {
  POSITION Position;
  Position.X = 0;
  Position.Y = 0;

  char * Pointer = NULL;
  Pointer = strtok(NULL, "@");
  Position.X = atoi(Pointer);
  Pointer = strtok(NULL, "@");
  Position.Y = atoi(Pointer);
  printf("X: %d Y: %d\n", Position.X, Position.Y);

  // Parsing and set cursor position
  gMouse->SetCurrentPosition (gMouse, Position);
}

void ClickParser (char * Input) {
  char * Pointer = NULL;

  printf("ClickParser begin...\n");
  printf("Input %s\n", Pointer);
  Pointer = strtok(NULL, "@");
  printf("Pointer %s\n", Pointer);
  gMouse->ClickEvent(gMouse, atoi(Pointer));
}

void ReleaseClickParser (char * Input) {
  char * Pointer = NULL;

  Pointer = strtok(NULL, "@");
  gMouse->ClickEvent(gMouse, atoi(Pointer));
}

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason, 
                   void *user, 
                   void *in, 
                   size_t len) {
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
      printf("Command: %s\n", in);
      char * Pointer = NULL;
      unsigned int Index = 0;

			memcpy(&Message.data[LWS_SEND_BUFFER_PRE_PADDING], in, len);
			Message.len = len;

      Pointer = strtok(in, "@");
      while(CommandMapping[Index].Command != NULL) {
        if (strcmp(Pointer, CommandMapping[Index].Command) == 0) {
          CommandMapping[Index].ExecuteCommand(Pointer);
          goto FINISH;
        }
        Index++;
      }

      printf("%s possible trash command\n",in);

			//lws_callback_on_writable_all_protocol(lws_get_context(wsi),lws_get_protocol(wsi));
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			lws_write( wsi, &Message.data[LWS_SEND_BUFFER_PRE_PADDING], Message.len, LWS_WRITE_TEXT );
			break;

		default:
			break;
	}

FINISH:

	return 0;
}
