#include "server.h"
#include "mouse.h"

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);

void PositionParser (char * Input);
void ClickParser (char * Input);
void ReleaseClickParser (char * Input);
