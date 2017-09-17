#include "server.h"
#include "keyboard.h"

#include <string.h>

extern KEYBOARD * gKeyboard;

static COMMAND_MAPPING CommandMapping[] = {
  {
    "PRESS",
    WriteParser
  },
  {
    NULL, NULL
  }
};

void WriteParser (char * Input) {
  char * Pointer = NULL;

  Pointer = strtok(NULL, "@");
  printf("WriterParser: %s\n", Pointer);

  gKeyboard->TypeLetter (gKeyboard, Pointer);
}

int KeyboardCallback (struct lws *wsi,
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
