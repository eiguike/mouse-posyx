#ifndef SERVER_H
#define SERVER_H

#include <libwebsockets.h>

#include "protocols/http_protocol.h"
#include "protocols/mouse_protocol.h"
#include "protocols/keyboard_protocol.h"

#define EXAMPLE_RX_BUFFER_BYTES (50)

typedef struct PROTOCOL_MAPPING {
	char * Command;
	void (*ExecuteCommand)();
} COMMAND_MAPPING;

int callback_mouse(struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);

struct payload {
	unsigned char data[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
	size_t len;
} Message;

typedef struct WEBSOCKET_SERVER {
  struct lws_context_creation_info * ContextInfo;
  struct lws_context * Context;
  short int IsStop;

  int (*Start)(struct WEBSOCKET_SERVER * this);
  int (*Stop)(struct WEBSOCKET_SERVER * this);
  void (*Dispose)(struct WEBSOCKET_SERVER ** this);
} SERVER;

int Start (SERVER * this);
int Stop (SERVER* this);
SERVER * InitializeServer(int Port);
SERVER * InitializeServerSSL(int Port);

#endif
