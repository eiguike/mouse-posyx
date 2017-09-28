#include "server.h"
#include <string.h>

int AboutCallback ( struct lws *wsi,
					enum lws_callback_reasons reason, 
					void *user, 
					void *in, 
					size_t len)
{
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
			strcpy(Message.data[LWS_SEND_BUFFER_PRE_PADDING], "TEXTO");
			lws_write( wsi, &Message.data[LWS_SEND_BUFFER_PRE_PADDING], 6, LWS_WRITE_TEXT);
			break;

		default:
			break;
	}
}
