#include <libwebsockets.h>
#include <string.h>
#include <signal.h>

#include "logger.h"
#include "server.h"
#include "protocols/http_protocol.h"

struct lws_protocols protocols[] =
{
  {
    "http-protocol",
		callback_http,
		sizeof(per_session_data__http),
		0,
  },
  {
    "mouse-protocol",
    callback_mouse,
    0,
    EXAMPLE_RX_BUFFER_BYTES,
  },
  {
    "keyboard-protocol",
    KeyboardCallback,
    0,
    EXAMPLE_RX_BUFFER_BYTES,
  },
  {
    "about-protocol",
    CallbackAbout,
    0,
    EXAMPLE_RX_BUFFER_BYTES,
  },
  { NULL, NULL, 0, 0 } /* terminator */
};

void Dispose(SERVER** This) {
  SERVER* Pointer = *This;
  if (Pointer != NULL) {
    if (Pointer->ContextInfo == NULL) {
      free(Pointer->ContextInfo);
    }
    if (Pointer->Context != NULL) {
      lws_context_destroy(Pointer->Context);
    }
    memset(Pointer, 0, sizeof(SERVER));
    free(Pointer);
    *This = NULL;
  }
}

int Start(SERVER * this) {
  lws_service(this->Context, 1000000);
}

int Stop(SERVER * this) {
  Logger->Info("Stopping LWS...");
  this->IsStop = 1;
  return 0;
}

SERVER * InitializeServer(int Port) {
  struct lws_context_creation_info* ContextInfo = NULL;
  struct lws_context * Context = NULL;
  SERVER* Return = NULL;

  ContextInfo = calloc(1, sizeof(struct lws_context_creation_info));
  if (ContextInfo == NULL) {
    Logger->Error(__FUNCTION__, __LINE__, "Could not allocate memory for ContextInfo");
    goto FAIL;
  }

  ContextInfo->port = Port;
  ContextInfo->protocols = protocols;
  ContextInfo->gid = -1;
  ContextInfo->uid = -1;

  Context = lws_create_context(ContextInfo);
  if (Context == NULL) {
    Logger->Error(__FUNCTION__, __LINE__,"Could not create context for LWS");
    goto FAIL;
  }

  Return = calloc(sizeof(SERVER), 1);
  if (Return == NULL) {
    Logger->Error(__FUNCTION__, __LINE__,"Could not initialize SERVER Data structure");
    goto FAIL;
  }

  Return->Start = Start;
  Return->Stop = Stop;
  Return->Dispose = Dispose;
  Return->ContextInfo = ContextInfo;
  Return->Context = Context;
  Return->IsStop = 0;

  goto FINISH;

FAIL:
  if (ContextInfo != NULL) {
    free(ContextInfo);
  }
  if (Context != NULL) {
    lws_context_destroy(Context);
  }

FINISH:
  return Return;
}

SERVER * InitializeServerSSL(int Port) {
  Logger->Info("Using SSL\n");

  int Options = 0;
  char cert_path[1024] = "./libwebsockets-test-server.pem";
  char key_path[1024] = "./libwebsockets-test-server.key.pem";
  char ca_path[1024] = ".";

  Options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
  Options |= LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS;

  Options |= LWS_SERVER_OPTION_VALIDATE_UTF8;
  Options |= LWS_SERVER_OPTION_EXPLICIT_VHOSTS;

  struct lws_context_creation_info ContextInfo;

  memset(&ContextInfo, 0, sizeof(ContextInfo));

  ContextInfo.ssl_cert_filepath = cert_path;
  ContextInfo.ssl_private_key_filepath = key_path;
  ContextInfo.ssl_ca_filepath = ca_path;
  ContextInfo.port = Port;
  ContextInfo.protocols = protocols;
  ContextInfo.gid = -1;
  ContextInfo.uid = -1;
  ContextInfo.options = Options;
  ContextInfo.ssl_cipher_list =  "ECDHE-ECDSA-AES256-GCM-SHA384:"
                                 "ECDHE-RSA-AES256-GCM-SHA384:"
                                 "DHE-RSA-AES256-GCM-SHA384:"
                                 "ECDHE-RSA-AES256-SHA384:"
                                 "HIGH:!aNULL:!eNULL:!EXPORT:"
                                 "!DES:!MD5:!PSK:!RC4:!HMAC_SHA1:"
                                 "!SHA1:!DHE-RSA-AES128-GCM-SHA256:"
                                 "!DHE-RSA-AES128-SHA256:"
                                 "!AES128-GCM-SHA256:"
                                 "!AES128-SHA256:"
                                 "!DHE-RSA-AES256-SHA256:"
                                 "!AES256-GCM-SHA384:"
                                 "!AES256-SHA256";

  //Server->ContextInfo = ContextInfo;
  struct lws_context * Context = lws_create_context(&ContextInfo);
  struct lws_vhost * VirtualHost = lws_create_vhost(Context, &ContextInfo);

  ContextInfo.port++;

  lws_init_vhost_client_ssl(&ContextInfo, VirtualHost);

  while(1) {
    lws_service(Context, 1000000);
  }

  lws_context_destroy(Context);
  lwsl_notice("libwebsockets-test-server exited cleanly\n");

FAIL:
  return NULL;
}

