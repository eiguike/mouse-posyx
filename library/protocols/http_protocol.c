/*
 * libwebsockets-test-server - libwebsockets test implementation
 *
 * Copyright (C) 2010-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#include "libwebsockets.h"
#include "protocols/http_protocol.h"

#define LWS_CB_REASON_AUX_BF__PROXY		2
char *resource_path = "./";

/*
 * This demo server shows how to use libwebsockets for one or more
 * websocket protocols in the same server
 *
 * It defines the following websocket protocols:
 *
 *  dumb-increment-protocol:  once the socket is opened, an incrementing
 *				ascii string is sent down it every 50ms.
 *				If you send "reset\n" on the websocket, then
 *				the incrementing number is reset to 0.
 *
 *  lws-mirror-protocol: copies any received packet to every connection also
 *				using this protocol, including the sender
 */


enum demo_protocols {
	/* always first */
	PROTOCOL_HTTP = 0,

	PROTOCOL_DUMB_INCREMENT,
	PROTOCOL_LWS_MIRROR,

	/* always last */
	DEMO_PROTOCOL_COUNT
};

/*
 * We take a strict whitelist approach to stop ../ attacks
 */
struct serveable {
	const char *urlpath;
	const char *mimetype;
};

/*
 * this is just an example of parsing handshake headers, you don't need this
 * in your code unless you will filter allowing connections by the header
 * content
 */
void
dump_handshake_info(struct lws *wsi)
{
	int n = 0, len;
	char buf[256];
	const unsigned char *c;

	do {
		c = lws_token_to_string(n);
		if (!c) {
			n++;
			continue;
		}

		len = lws_hdr_total_length(wsi, n);
		if (!len || len > sizeof(buf) - 1) {
			n++;
			continue;
		}

		lws_hdr_copy(wsi, buf, sizeof buf, n);
		buf[sizeof(buf) - 1] = '\0';

		fprintf(stderr, "    %s = %s\n", (char *)c, buf);
		n++;
	} while (c);
}

const char * get_mimetype(const char *file)
{
	int n = strlen(file);

	if (n < 5)
		return NULL;

	if (!strcmp(&file[n - 4], ".ico"))
		return "image/x-icon";

	if (!strcmp(&file[n - 4], ".png"))
		return "image/png";

	if (!strcmp(&file[n - 5], ".html"))
		return "text/html";

	if (!strcmp(&file[n - 4], ".css"))
		return "text/css";

	if (!strcmp(&file[n - 3], ".js"))
		return "text/javascript";

	return NULL;
}


static const char * const param_names[] = {
	"text",
	"send",
	"file",
	"upload",
};

enum enum_param_names {
	EPN_TEXT,
	EPN_SEND,
	EPN_FILE,
	EPN_UPLOAD,
};

static int
file_upload_cb(void *data, const char *name, const char *filename,
	       char *buf, int len, enum lws_spa_fileupload_states state)
{
	per_session_data__http *pss =
			(per_session_data__http *)data;
	int n;

	(void)n;

	switch (state) {
    case LWS_UFS_OPEN:
		strncpy(pss->filename, filename, sizeof(pss->filename) - 1);
		/* we get the original filename in @filename arg, but for
		 * simple demo use a fixed name so we don't have to deal with
		 * attacks  */
		pss->post_fd = (lws_filefd_type)open("/tmp/post-file",
			       O_CREAT | O_TRUNC | O_RDWR, 0600);
		break;
    case LWS_UFS_FINAL_CONTENT:
    case LWS_UFS_CONTENT:
		if (len) {
			pss->file_length += len;

			/* if the file length is too big, drop it */
			if (pss->file_length > 100000)
				return 1;

			n = write((int)pss->post_fd, buf, len);
			lwsl_notice("%s: write %d says %d\n", __func__, len, n);
		}
		if (state == LWS_UFS_CONTENT)
			break;
		close((int)pss->post_fd);
		pss->post_fd = LWS_INVALID_FILE;
		break;
	}

	return 0;
}

/* this protocol server (always the first one) handles HTTP,
 *
 * Some misc callbacks that aren't associated with a protocol also turn up only
 * here on the first protocol server.
 */

int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user,
		  void *in, size_t len)
{
	per_session_data__http *pss =
			(per_session_data__http *)user;
	unsigned char buffer[4096 + LWS_PRE];
       lws_filepos_t amount, file_len, sent;
	char leaf_path[1024];
	const char *mimetype;
	char *other_headers;
	unsigned char *end, *start;
	struct timeval tv;
	unsigned char *p;
#ifndef LWS_NO_CLIENT
	per_session_data__http *pss1;
	struct lws *wsi1;
#endif
	char buf[256];
	char b64[64];
	int n, m;
#ifdef EXTERNAL_POLL
	struct lws_pollargs *pa = (struct lws_pollargs *)in;
#endif


	switch (reason) {
    case LWS_CALLBACK_HTTP:

		lwsl_info("lws_http_serve: %s\n", (const char *)in);

		if (lws_get_peer_simple(wsi, buf, sizeof(buf)))
			lwsl_info("HTTP connect from %s\n", buf);

		if (len < 1) {
			lws_return_http_status(wsi,
						HTTP_STATUS_BAD_REQUEST, NULL);
			goto try_to_reuse;
		}

//#if 1
//		/* this example server has no concept of directories */
//		if (strchr((const char *)in + 1, '/')) {
//			lws_return_http_status(wsi, HTTP_STATUS_NOT_ACCEPTABLE, NULL);
//			goto try_to_reuse;
//		}
//#endif

		/* if a legal POST URL, let it continue and accept data */
		if (lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI))
			return 0;

    strcpy(buf, resource_path);

		if (strcmp(in, "/")) {
			if (*((const char *)in) != '/')
				strcat(buf, "/");
			strncat(buf, in, sizeof(buf) - strlen(buf) - 1);
		} else { /* default file to serve */
			strcat(buf, "/index.html");
    }
		buf[sizeof(buf) - 1] = '\0';

		/* refuse to serve files we don't understand */
		mimetype = get_mimetype(buf);
		if (!mimetype) {
			lwsl_err("Unknown mimetype for %s\n", buf);
			lws_return_http_status(wsi,
				      HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, "Unknown Mimetype");
			return -1;
		}

		/* demonstrates how to set a cookie on / */

		other_headers = leaf_path;
		p = (unsigned char *)leaf_path;
		if (!strcmp((const char *)in, "/") &&
			   !lws_hdr_total_length(wsi, WSI_TOKEN_HTTP_COOKIE)) {
			/* this isn't very unguessable but it'll do for us */
			gettimeofday(&tv, NULL);
			n = sprintf(b64, "test=LWS_%u_%u_COOKIE;Max-Age=360000",
				(unsigned int)tv.tv_sec,
				(unsigned int)tv.tv_usec);

			if (lws_add_http_header_by_name(wsi,
				(unsigned char *)"set-cookie:",
				(unsigned char *)b64, n, &p,
				(unsigned char *)leaf_path + sizeof(leaf_path)))
				return 1;
		}
#if !defined(LWS_WITH_HTTP2)
		if (lws_is_ssl(wsi) && lws_add_http_header_by_name(wsi,
						(unsigned char *)
						"Strict-Transport-Security:",
						(unsigned char *)
						"max-age=15768000 ; "
						"includeSubDomains", 36, &p,
						(unsigned char *)leaf_path +
							sizeof(leaf_path)))
			return 1;
#endif
		n = (char *)p - leaf_path;

		n = lws_serve_http_file(wsi, buf, mimetype, other_headers, n);
		if (n < 0)
			return -1; /* error*/

		/*
		 * notice that the sending of the file completes asynchronously,
		 * we'll get a LWS_CALLBACK_HTTP_FILE_COMPLETION callback when
     * it's done.  That's the case even if we just completed the
     printf("even\n");*
     printf("even\n");*
		 * send, so wait for that.
		 */
		break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
                ((char *)in)[len] = '\0';
                lwsl_info("rx %d '%s'\n", (int)len, (char *)in);
                break;

    case LWS_CALLBACK_HTTP_BODY:
		/* create the POST argument parser if not already existing */
		if (!pss->spa) {
			pss->spa = lws_spa_create(wsi, param_names,
					ARRAY_SIZE(param_names), 1024,
					file_upload_cb, pss);
			if (!pss->spa)
				return -1;

			pss->filename[0] = '\0';
			pss->file_length = 0;
		}

		/* let it parse the POST data */
		if (lws_spa_process(pss->spa, in, len))
			return -1;
		break;

    case LWS_CALLBACK_HTTP_BODY_COMPLETION:
		lwsl_debug("LWS_CALLBACK_HTTP_BODY_COMPLETION\n");
		/*
		 * the whole of the sent body arrived,
		 * respond to the client with a redirect to show the
		 * results
		 */

		/* call to inform no more payload data coming */
		lws_spa_finalize(pss->spa);

		p = (unsigned char *)pss->result + LWS_PRE;
		end = p + sizeof(pss->result) - LWS_PRE - 1;
		p += sprintf((char *)p,
			"<html><body><h1>Form results (after urldecoding)</h1>"
			"<table><tr><td>Name</td><td>Length</td><td>Value</td></tr>");

		for (n = 0; n < ARRAY_SIZE(param_names); n++)
			p += lws_snprintf((char *)p, end - p,
				    "<tr><td><b>%s</b></td><td>%d</td><td>%s</td></tr>",
				    param_names[n],
				    lws_spa_get_length(pss->spa, n),
				    lws_spa_get_string(pss->spa, n));

		p += lws_snprintf((char *)p, end - p, "</table><br><b>filename:</b> %s, <b>length</b> %ld",
				pss->filename, pss->file_length);

		p += lws_snprintf((char *)p, end - p, "</body></html>");
		pss->result_len = p - (unsigned char *)(pss->result + LWS_PRE);

		p = buffer + LWS_PRE;
		start = p;
		end = p + sizeof(buffer) - LWS_PRE;

		if (lws_add_http_header_status(wsi, HTTP_STATUS_OK, &p, end))
			return 1;

		if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_TYPE,
				(unsigned char *)"text/html", 9, &p, end))
			return 1;
		if (lws_add_http_header_content_length(wsi, pss->result_len, &p, end))
			return 1;
		if (lws_finalize_http_header(wsi, &p, end))
			return 1;

		n = lws_write(wsi, start, p - start, LWS_WRITE_HTTP_HEADERS);
		if (n < 0)
			return 1;

		n = lws_write(wsi, (unsigned char *)pss->result + LWS_PRE,
			      pss->result_len, LWS_WRITE_HTTP);
		if (n < 0)
			return 1;
		goto try_to_reuse;
    case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
		lwsl_debug("LWS_CALLBACK_HTTP_DROP_PROTOCOL\n");

		/* called when our wsi user_space is going to be destroyed */
		if (pss->spa) {
			lws_spa_destroy(pss->spa);
			pss->spa = NULL;
		}
		break;
    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
		goto try_to_reuse;

    case LWS_CALLBACK_HTTP_WRITEABLE:
		lwsl_info("LWS_CALLBACK_HTTP_WRITEABLE: %p\n", wsi);

		if (pss->client_finished)
			return -1;

		if (!lws_get_child(wsi) && !pss->fop_fd) {
			lwsl_notice("fop_fd NULL\n");
			goto try_to_reuse;
		}

#ifndef LWS_NO_CLIENT
		if (pss->reason_bf & LWS_CB_REASON_AUX_BF__PROXY) {
			char *px = buf + LWS_PRE;
			int lenx = sizeof(buf) - LWS_PRE;
			/*
			 * our sink is writeable and our source has something
			 * to read.  So read a lump of source material of
			 * suitable size to send or what's available, whichever
			 * is the smaller.
			 */


			pss->reason_bf &= ~LWS_CB_REASON_AUX_BF__PROXY;
			wsi1 = lws_get_child(wsi);
			if (!wsi1)
				break;
			if (lws_http_client_read(wsi1, &px, &lenx) < 0)
				return -1;

			if (pss->client_finished)
				return -1;

			break;
		}

		if (lws_get_child(wsi))
			break;

#endif
		/*
		 * we can send more of whatever it is we were sending
		 */
		sent = 0;
		do {
			/* we'd like the send this much */
			n = sizeof(buffer) - LWS_PRE;

			/* but if the peer told us he wants less, we can adapt */
			m = lws_get_peer_write_allowance(wsi);

			/* -1 means not using a protocol that has this info */
			if (m == 0)
				/* right now, peer can't handle anything */
				goto later;

			if (m != -1 && m < n)
				/* he couldn't handle that much */
				n = m;

			n = lws_vfs_file_read(pss->fop_fd,
					       &amount, buffer + LWS_PRE, n);
			/* problem reading, close conn */
			if (n < 0) {
				lwsl_err("problem reading file\n");
				goto bail;
			}
			n = (int)amount;
			/* sent it all, close conn */
			if (n == 0)
				goto penultimate;
			/*
			 * To support HTTP2, must take care about preamble space
			 *
			 * identification of when we send the last payload frame
			 * is handled by the library itself if you sent a
			 * content-length header
			 */
			m = lws_write(wsi, buffer + LWS_PRE, n, LWS_WRITE_HTTP);
			if (m < 0) {
				lwsl_err("write failed\n");
				/* write failed, close conn */
				goto bail;
			}
			if (m) /* while still active, extend timeout */
				lws_set_timeout(wsi, PENDING_TIMEOUT_HTTP_CONTENT, 5);
			sent += m;

		} while (!lws_send_pipe_choked(wsi) && (sent < 1024 * 1024));
later:
		lws_callback_on_writable(wsi);
		break;
penultimate:
		lws_vfs_file_close(&pss->fop_fd);
		pss->fop_fd = NULL;
		goto try_to_reuse;

bail:
		lws_vfs_file_close(&pss->fop_fd);

		return -1;

	/*
	 * callback for confirming to continue with client IP appear in
	 * protocol 0 callback since no websocket protocol has been agreed
	 * yet.  You can just ignore this if you won't filter on client IP
	 * since the default unhandled callback return is 0 meaning let the
	 * connection continue.
	 */
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
		/* if we returned non-zero from here, we kill the connection */
		break;

#ifndef LWS_NO_CLIENT
    case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP: {
		char ctype[64], ctlen = 0;
		lwsl_err("LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP\n");
		p = buffer + LWS_PRE;
		end = p + sizeof(buffer) - LWS_PRE;
		if (lws_add_http_header_status(lws_get_parent(wsi), HTTP_STATUS_OK, &p, end))
			return 1;
		if (lws_add_http_header_by_token(lws_get_parent(wsi),
				WSI_TOKEN_HTTP_SERVER,
			    	(unsigned char *)"libwebsockets",
				13, &p, end))
			return 1;

		ctlen = lws_hdr_copy(wsi, ctype, sizeof(ctype), WSI_TOKEN_HTTP_CONTENT_TYPE);
		if (ctlen > 0) {
			if (lws_add_http_header_by_token(lws_get_parent(wsi),
				WSI_TOKEN_HTTP_CONTENT_TYPE,
				(unsigned char *)ctype, ctlen, &p, end))
				return 1;
		}
#if 0
		if (lws_add_http_header_content_length(lws_get_parent(wsi),
						       file_len, &p, end))
			return 1;
#endif
		if (lws_finalize_http_header(lws_get_parent(wsi), &p, end))
			return 1;

		*p = '\0';
		lwsl_info("%s\n", buffer + LWS_PRE);

		n = lws_write(lws_get_parent(wsi), buffer + LWS_PRE,
			      p - (buffer + LWS_PRE),
			      LWS_WRITE_HTTP_HEADERS);
		if (n < 0)
			return -1;

		break; }
    case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
		//lwsl_err("LWS_CALLBACK_CLOSED_CLIENT_HTTP\n");
		return -1;
		break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
		//lwsl_err("LWS_CALLBACK_RECEIVE_CLIENT_HTTP: wsi %p\n", wsi);
		assert(lws_get_parent(wsi));
		if (!lws_get_parent(wsi))
			break;
		pss1 = lws_wsi_user(lws_get_parent(wsi));
		pss1->reason_bf |= LWS_CB_REASON_AUX_BF__PROXY;
		lws_callback_on_writable(lws_get_parent(wsi));
		break;
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
		//lwsl_err("LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ len %d\n", (int)len);
		assert(lws_get_parent(wsi));
		m = lws_write(lws_get_parent(wsi), (unsigned char *)in,
				len, LWS_WRITE_HTTP);
		if (m < 0)
			return -1;
		break;
    case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
		//lwsl_err("LWS_CALLBACK_COMPLETED_CLIENT_HTTP\n");
		assert(lws_get_parent(wsi));
		if (!lws_get_parent(wsi))
			break;
		pss1 = lws_wsi_user(lws_get_parent(wsi));
		pss1->client_finished = 1;
		break;
#endif

	/*
	 * callbacks for managing the external poll() array appear in
	 * protocol 0 callback
	 */

    case LWS_CALLBACK_LOCK_POLL:
		break;

    case LWS_CALLBACK_UNLOCK_POLL:
		break;

#ifdef EXTERNAL_POLL
    case LWS_CALLBACK_ADD_POLL_FD:

		if (count_pollfds >= max_poll_elements) {
			lwsl_err("LWS_CALLBACK_ADD_POLL_FD: too many sockets to track\n");
			return 1;
		}

		fd_lookup[pa->fd] = count_pollfds;
		pollfds[count_pollfds].fd = pa->fd;
		pollfds[count_pollfds].events = pa->events;
		pollfds[count_pollfds++].revents = 0;
		break;

    case LWS_CALLBACK_DEL_POLL_FD:
		if (!--count_pollfds)
			break;
		m = fd_lookup[pa->fd];
		/* have the last guy take up the vacant slot */
		pollfds[m] = pollfds[count_pollfds];
		fd_lookup[pollfds[count_pollfds].fd] = m;
		break;

    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
	        pollfds[fd_lookup[pa->fd]].events = pa->events;
		break;
#endif

    case LWS_CALLBACK_GET_THREAD_ID:
		/*
		 * if you will call "lws_callback_on_writable"
		 * from a different thread, return the caller thread ID
		 * here so lws can use this information to work out if it
		 * should signal the poll() loop to exit and restart early
		 */

		/* return pthread_getthreadid_np(); */

		break;


	default:
		break;
	}

	return 0;

	/* if we're on HTTP1.1 or 2.0, will keep the idle connection alive */
try_to_reuse:
	if (lws_http_transaction_completed(wsi))
		return -1;

	return 0;
}
