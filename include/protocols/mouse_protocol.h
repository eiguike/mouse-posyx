#include "server.h"
#include "mouse.h"

void PositionParser (char * Input, POSITION * Position);

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);

typedef struct MOUSE_PROTOCOL_MAPPING {
	char * Command;
	void (*ExecuteCommand)();
} COMMAND_MAPPING;

void PositionParser (char * Input);
void ClickParser (char * Input);
void ReleaseClickParser (char * Input);