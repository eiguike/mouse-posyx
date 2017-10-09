#include "server.h"

int CallbackHttp( struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void *user,
                   void *in,
                   size_t len );
