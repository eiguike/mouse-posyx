#include <libwebsockets.h>
#include <string.h>

#include "server.h"

struct lws_protocols protocols[] =
{
  /* The first protocol must always be the HTTP handler */
  {
    "http-only",   /* name */
    callback_http, /* callback */
    0,             /* No per session data. */
    0,             /* max frame size / rx buffer */
  },
  {
    "mouse-protocol",
    callback_mouse,
    0,
    EXAMPLE_RX_BUFFER_BYTES,
  },
  { NULL, NULL, 0, 0 } /* terminator */
};

int Start(SERVER * this) {
  while(!this->IsStop){
    lws_service(this->Context, 1000000);
  }

  this->IsStop = 1;

  return 0;
}

int Stop(SERVER * this) {
  /*
     this->IsStop = 1;

     while(this->IsStop) {
  //sleep(1);
  }
  lws_context_destroy(this->Context);
  free(this->ContextInfo);
  free(this);

  return 0;
  */
}

SERVER * InitializeServer(int Port) {
  struct lws_context_creation_info ContextInfo;

  memset(&ContextInfo, 0, sizeof(ContextInfo));

  ContextInfo.port = Port;
  ContextInfo.protocols = protocols;
  ContextInfo.gid = -1;
  ContextInfo.uid = -1;

  //Server->ContextInfo = ContextInfo;
  struct lws_context * Context = lws_create_context(&ContextInfo);
  while(1) {
    lws_service(Context, 1000000);
  }

  lws_context_destroy(Context);

FAIL:
  return NULL;
}
