#include "server.h"

typedef struct buffer {
	//unsigned char data[ LWS_SEND_BUFFER_PRE_PADDING +
  //                    EXAMPLE_RX_BUFFER_BYTES +
  //                    LWS_SEND_BUFFER_POST_PADDING ];
  unsigned char * Data
	size_t Length;
} BUFFER;

typedef struct Information {
  char * Name;
  char * Linux;
  char * ApplicationVersion;
} INFORMATION;

int AboutCallback (struct lws *wsi,
                   enum lws_callback_reasons reason,
                   void * user,
                   void * in,
                   size_t len);

void InitializeAboutProtocol ();
