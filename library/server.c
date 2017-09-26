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
  {
    "keyboard-protocol",
    KeyboardCallback,
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


SERVER * InitializeServerSSL(int Port) {
  printf("Using SSL\n");
  int Options = 0;
  char cert_path[1024] = "cert.pem";
  char key_path[1024] = "key.pem";
  char ca_path[1024] = "";

//#ifdef USE_SSL
  Options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
  Options |= LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS;
  Options |= LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT;
//#endif

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
  lws_init_vhost_client_ssl(&ContextInfo, VirtualHost);

  while(1) {
    lws_service(Context, 1000000);
  }

  lws_context_destroy(Context);
  lwsl_notice("libwebsockets-test-server exited cleanly\n");

FAIL:
  return NULL;
}

