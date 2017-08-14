#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EXAMPLE_RX_BUFFER_BYTES (50)

int callback_http(struct lws *wsi,
                  enum lws_callback_reasons reason,
                  void * user,
                  void *in,
                  size_t len);

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);

typedef struct WEBSOCKET_SERVER {
  struct lws_protocols * Protocols;
  struct lws_context_creation_info * ContextInfo;
  struct lws_context * Context;
  unsigned int NumbersOfProtocols;
  short int IsStop;

  int (*Start)(struct WEBSOCKET_SERVER * this);
  int (*Stop)(struct WEBSOCKET_SERVER * this);
} SERVER;



struct payload {
	unsigned char data[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
	size_t len;
} received_payload;

int Start (SERVER * this);
int Stop (SERVER * this);
SERVER * InitializeServer(int Port);
