#include <string.h>

#include "logger.h"
#include "server.h"
#include "keyboard.h"

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
  Logger->Info("WriterParser: %s", Pointer);

  gKeyboard->TypeLetter (gKeyboard, Pointer);
}

int KeyboardCallback (struct lws *wsi,
                      enum lws_callback_reasons reason,
                      void *user,
                      void *in,
                      size_t len) {
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
      Logger->Info("Command: %s", in);
      char * Pointer = NULL;

			memcpy(&Message.data[LWS_SEND_BUFFER_PRE_PADDING], in, len);
			Message.len = len;

      Pointer = strtok(in, "@");
      IterateCommandMapping(CommandMapping, Pointer)(Pointer)
      Logger->Info("%s possible trash command",in);

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
