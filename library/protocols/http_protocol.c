#include "server.h"

int CallbackHttp( struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void *user,
                   void *in,
                   size_t len )
{
  switch(reason) {
    case LWS_CALLBACK_HTTP:
      lws_serve_http_file( wsi, "index.html", "text/html", NULL, 0 );
      break;
    default:
      break;
  }
  return 0;
}
