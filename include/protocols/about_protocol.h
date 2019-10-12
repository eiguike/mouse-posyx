#include "server.h"

typedef struct Information {
  char * Name;
  char * Linux;
  char * ApplicationVersion;
} INFORMATION;

int CallbackAbout (struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);
