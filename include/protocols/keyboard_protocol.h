#include "server.h"

int KeyboardCallback (struct lws *wsi,
                      enum lws_callback_reasons reason,
                      void * user,
                      void * in,
                      size_t len);

void WriteParser (char * Input);
