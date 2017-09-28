#include <libwebsockets.h>
#include <string.h>

#include "server.h"

struct lws_protocols protocols[] =
{
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

