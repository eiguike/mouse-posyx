#include "server.h"
#include "mouse.h"

#include <string.h>

extern MOUSE * gMouse;

void PositionParser (char * Input, POSITION * Position) {
  char * pointer = NULL;
  printf("INPUT: %s\n", Input);

  pointer = strtok(Input, "@");
  Position->X = atoi(pointer);
  pointer = strtok(NULL, "@");
  Position->Y = atoi(pointer);
  printf("X: %d Y: %d\n", Position->X, Position->Y);
}

int callback_mouse( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
  POSITION Position;
  Position.X = 0;
  Position.Y = 0;
	switch(reason)
	{
		case LWS_CALLBACK_RECEIVE:
			memcpy( &Message.data[LWS_SEND_BUFFER_PRE_PADDING], in, len );
			Message.len = len;

      if (strcmp(in, "lokura") == 0) {
        gMouse->ClickEvent (gMouse, 1);
        break;
      } else if (strcmp(in, "hue") == 0) {
        gMouse->ClickEvent (gMouse, 3);
      }

      // Parsing and set cursor position
      PositionParser (in, &Position);
      gMouse->SetCurrentPosition (gMouse, Position);

			//lws_callback_on_writable_all_protocol(lws_get_context(wsi),lws_get_protocol(wsi));
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			lws_write( wsi, &Message.data[LWS_SEND_BUFFER_PRE_PADDING], Message.len, LWS_WRITE_TEXT );
			break;

		default:
			break;
	}

	return 0;
}
