#include "server.h"
#include "logger.h"

#define APPLICATION_VERSION "mouse-posyx 1.0v"

INFORMATION Computer;

int CallbackAbout( struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len )
{
  return 0;
}
