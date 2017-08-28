#include "server.h"
#include "mouse.h"

void PositionParser (char * Input, POSITION * Position);

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);
