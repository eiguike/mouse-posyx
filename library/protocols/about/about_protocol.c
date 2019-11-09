#include "server.h"
#include "logger.h"

#define APPLICATION_VERSION "mouse-posyx 1.0v"

INFORMATION Computer;

static COMMAND_MAPPING CommandMapping[] = {
  {
    "ABOUT",
    WriteParser
  },
  {
    NULL, NULL
  }
};

int AboutCallback( struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len )
{

  switch(reason) {
    case LWS_CALLBACK_RECEIVE:
      Logger->Info("AboutCallback received command %s", in);
      break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
      break;
    default:
      break;
  }

  return 0;
}
