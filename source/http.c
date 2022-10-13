#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "libtools/http.h"
#include "libtools/recvline.h"
#include "libtools/string.h"
#include "libtools/tools.h"
#include "libtools/trim.h"

/*------------------------------------------------------------------------*/

#define __HTTPEOL "\r\n"

static char httpeol[] = __HTTPEOL;
static size_t httpeol_len = countof(httpeol) - 1;

static const char httpver[] = "HTTP/";

/*------------------------------------------------------------------------*/

/**
 * @brief convert socket return code to HTTP
 * @param [in] i return code from recv() or send()
 * @return HTTP status code
 */
static http_status_t sock2http_status(ssize_t i)
{
	switch (i) {
		case 0:
			return (HTTP_CONNECTION_CLOSED);

		case -1:
			return (HTTP_SOCKET_ERROR);

		default:
			return (HTTP_SUCCESS);
	}
}

/*------------------------------------------------------------------------*/

/**
 * @brief send HTTP headers
 * @param [in] sockfd socket descriptor
 * @param [in] hdr HTTP headers
 * @param [in] count count of @p hdr
 * @return HTTP status code
 * @retval HTTP_OUT_MEMORY can't complete request
 */
static http_status_t http_headers_send(int sockfd, http_header_t hdr[], size_t count)
{
	char *s;
	int len;

	/* send headers */
	for (size_t i = 0; i < count; ++ i) {
		/* serialize header to string */
		switch (hdr[i].type) {
			case HTTP_HEADER_STRING:
				len = asprintf(&s, "%s: %s" __HTTPEOL,
					hdr[i].name, hdr[i].val.s
				);
				break;

			case HTTP_HEADER_INT:
				len = asprintf(&s, "%s: %d" __HTTPEOL,
					hdr[i].name, hdr[i].val.i
				);
				break;

			default:
				return (HTTP_INVALID_HEADER);
		}

		if (-1 == len) {
			return (HTTP_OUT_MEMORY);
		}

		http_status_t ret;

		ret = sock2http_status(send(sockfd, s, len, 0));
		free(s);

		if (HTTP_ERROR(ret))
			return (ret);
	}

	/* send empty line to finish */
	return (sock2http_status(send(sockfd, httpeol, httpeol_len, 0)));
}

/*------------------------------------------------------------------------*/

/**
 * @brief return text representation of HTTP method
 * @param [in] method method
 * @param [out] m string
 * @return pointer to @p
 * @retval NULL unknown HTTP method
 */
static const char *http_method_str(http_method_t method, const char **m)
{
	assert(m);

	switch (method) {
		case HTTP_OPTIONS:
			*m = "OPTIONS";
			break;

		case HTTP_GET:
			*m = "GET";
			break;

		case HTTP_HEAD:
			*m = "HEAD";
			break;

		case HTTP_POST:
			*m = "POST";
			break;

		case HTTP_PUT:
			*m = "PUT";
			break;

		case HTTP_DELETE:
			*m = "DELETE";
			break;

		case HTTP_TRACE:
			*m = "TRACE";
			break;

		case HTTP_CONNECT:
			*m = "CONNECT";
			break;

		default:
			*m = NULL;
			break;
	}

	return (*m);
}

/*------------------------------------------------------------------------*/

/**
 * @brief parse string as HTTP request
 * @param [in] s pointer to string
 * @param [out] m HTTP method
 * @param [out] path pointer to string with parsed resource
 * @param [out] ver pointer to string with parsed version
 * @return HTTP status code
 * @retval HTTP_INVALID_REQUEST invalid data in stream
 * @retval HTTP_UNKNOWN_REQUEST unknown HTTP request
 */
static http_status_t http_request_parse(char *s, http_method_t *m, char **path, char **ver)
{
	assert(s);
	assert(m);
	assert(path);
	assert(ver);

	char *method;
	char *saveptr;

	/* parse HTTP request */
	if (!(method = strtok_r(s, " ", &saveptr))) {
		return (HTTP_INVALID_REQUEST);
	}

	if (!(*path = strtok_r(NULL, " ", &saveptr))) {
		return (HTTP_INVALID_REQUEST);
	}

	/* URI should begin with a slash */
	if ('/' != **path) {
		return (HTTP_INVALID_REQUEST);
	}

	if (!(*ver = strtok_r(NULL, " ", &saveptr))) {
		return (HTTP_INVALID_REQUEST);
	}

	/* somethings else? wrong request! */
	if (strtok_r(NULL, " ", &saveptr)) {
		return (HTTP_INVALID_REQUEST);
	}

	/* check HTTP version signature */
	if (!str_startswith(*ver, httpver)) {
		return (HTTP_INVALID_REQUEST);
	}

	/* parse HTTP method */
	if (!strcmp(method, "OPTIONS")) {
		*m = HTTP_OPTIONS;
	} else if (!strcmp(method, "GET")) {
		*m = HTTP_GET;
	} else if (!strcmp(method, "HEAD")) {
		*m = HTTP_HEAD;
	} else if (!strcmp(method, "POST")) {
		*m = HTTP_POST;
	} else if (!strcmp(method, "PUT")) {
		*m = HTTP_PUT;
	} else if (!strcmp(method, "DELETE")) {
		*m = HTTP_DELETE;
	} else if (!strcmp(method, "TRACE")) {
		*m = HTTP_TRACE;
	} else if (!strcmp(method, "CONNECT")) {
		*m = HTTP_CONNECT;
	} else {
		return (HTTP_UNKNOWN_REQUEST);
	}

	/* cleanup strings */
	rtrim(*ver);

	return (HTTP_SUCCESS);
}

/*------------------------------------------------------------------------*/

/**
 * @brief parse HTTP reply string
 * @param [in] s string to parse
 * @param [out] ver version string
 * @param [out] code HTTP code
 * @param [out] msg HTTP message
 * @return HTTP status code
 * @retval HTTP_INVALID_REPLY
 */
static http_status_t http_reply_parse(char *s, char **ver, int *code, char **msg)
{
	assert(s);
	assert(ver);
	assert(code);
	assert(msg);

	char *http_code;
	char *saveptr;

	/* parse HTTP request */
	if (!(*ver = strtok_r(s, " ", &saveptr))) {
		return (HTTP_INVALID_REPLY);
	}

	if (!(http_code = strtok_r(NULL, " ", &saveptr))) {
		return (HTTP_INVALID_REPLY);
	}

	if (!(*msg = strtok_r(NULL, " ", &saveptr))) {
		return (HTTP_INVALID_REPLY);
	}

	/* somethings else? wrong request! */
	if (strtok_r(NULL, " ", &saveptr)) {
		return (HTTP_INVALID_REPLY);
	}

	/* check HTTP version signature */
	if (!str_startswith(*ver, httpver)) {
		return (HTTP_INVALID_REPLY);
	}

	/* cleanup strings */
	rtrim(*msg);

	*code = atoi(http_code);

	return (HTTP_SUCCESS);
}

/*------------------------------------------------------------------------*/

/**
 * @brief parse string as a HTTP header
 * @param [in] s string to parse
 * @param [out] name pointer to header name
 * @param [out] val pointer to header value
 * @return HTTP status code
 */
static http_status_t http_header_parse(char *s, char **name, char **val)
{
	assert(s);
	assert(name);
	assert(val);

	/* header should be split by ':' */
	if (!(*val = strchr(s, ':'))) {
		return (HTTP_INVALID_HEADER);
	}

	*name = s;

	**val = '\0';
	++ *val;

	/* cleanup strings */
	trim(*name);
	trim(*val);

	return (HTTP_SUCCESS);
}

/*------------------------------------------------------------------------*/

/**
 * @brief read HTTP string
 * @param [in] sockfd socket descriptor
 * @param [in] s pointer for string
 * @param [in,out] len size of buffer at pointer @p s
 * @return status code
 * @retval HTTP_LARGE_REQUEST read of string not complete
 */
static http_status_t http_readline(int sockfd, char *s, size_t *len)
{
	assert(s);
	assert(len);

	http_status_t rc;
	ssize_t recvlen;

	recvlen = recvline(sockfd, s, *len, httpeol, httpeol_len);

	if (HTTP_ERROR(rc = sock2http_status(recvlen))) {
		return (rc);
	}

	*len = recvlen;

	/* verify end of line */
	if (recvlen < (ssize_t)httpeol_len ||
		memcmp(&s[recvlen - httpeol_len], httpeol, httpeol_len) != 0
	) {
		return (HTTP_LARGE_REQUEST);
	}

	*len -= httpeol_len;
	s[*len] = 0;

	return (HTTP_SUCCESS);
}

/*------------------------------------------------------------------------*/

http_status_t http_request_send(int sockfd, http_method_t method, const char *path, const char *ver, http_header_t hdr[], size_t count)
{
	assert(path);
	assert(ver);

	http_status_t ret;
	const char *m;
	char *s;
	int len;

	if (!http_method_str(method, &m)) {
		return (HTTP_UNKNOWN_REQUEST);
	}

	/* check HTTP version signature */
	if (!str_startswith(ver, httpver)) {
		return (HTTP_INVALID_REQUEST);
	}

	/* URI should begin with a slash */
	if ('/' != *path) {
		return (HTTP_INVALID_REQUEST);
	}

	if (-1 == (len = asprintf(&s, "%s %s %s" __HTTPEOL, m, path, ver))) {
		return (HTTP_OUT_MEMORY);
	}

	/* send HTTP request */
	ret = sock2http_status(send(sockfd, s, len, 0));
	free(s);

	if (HTTP_ERROR(ret))
		return (ret);

	/* send headers */
	return (http_headers_send(sockfd, hdr, count));
}

/*------------------------------------------------------------------------*/

http_status_t http_request_recv(int sockfd, http_request_cb_t request_cb, http_header_cb_t header_cb, void *priv)
{
	assert(request_cb);
	assert(header_cb);

	http_status_t ret;
	char s[PATH_MAX];
	size_t len;

	/* read first string */
	len = sizeof(s);
	if (HTTP_ERROR(ret = http_readline(sockfd, s, &len))) {
		return (ret);
	}

	http_method_t m;
	char *path, *ver;

	/* parse first string as request */
	if (HTTP_ERROR(ret = http_request_parse(s, &m, &path, &ver))) {
		return (ret);
	}

	if (request_cb && request_cb(m, path, ver, priv)) {
		return (HTTP_ABORTED);
	}

	char *header, *value;

	/* parse headers */
	for (;;) {
		len = sizeof(s);
		if (HTTP_ERROR(ret = http_readline(sockfd, s, &len))) {
			return (ret);
		}

		if (!len) {
			break;
		}

		if (HTTP_ERROR(ret = http_header_parse(s, &header, &value))) {
			return (ret);
		}

		if (header_cb && header_cb(header, value, priv)) {
			return (HTTP_ABORTED);
		}
	}

	return (HTTP_SUCCESS);
}

/*------------------------------------------------------------------------*/

http_status_t http_reply_send(int sockfd, const char *ver, int code, const char *desc, http_header_t hdr[], size_t count)
{
	assert(ver);
	assert(100 <= code && code < 600);
	assert(desc);

	http_status_t ret;
	char *s;
	int len;

	/* check HTTP version signature */
	if (!str_startswith(ver, httpver)) {
		return (HTTP_INVALID_REQUEST);
	}

	/* format HTTP reply */
	if (-1 == (len = asprintf(&s, "%s %d %s" __HTTPEOL, ver, code, desc))
	) {
		return (HTTP_OUT_MEMORY);
	}

	/* send HTTP reply */
	ret = sock2http_status(send(sockfd, s, len, 0));
	free(s);

	if (HTTP_ERROR(ret))
		return (ret);

	/* send headers */
	return (http_headers_send(sockfd, hdr, count));
}

/*------------------------------------------------------------------------*/

http_status_t http_reply_recv(int sockfd, http_reply_cb_t reply_cb, http_header_cb_t header_cb, void *priv)
{
	assert(reply_cb);
	assert(header_cb);

	http_status_t ret;
	char s[PATH_MAX];
	size_t len;

	/* read reply string */
	len = sizeof(s);
	if (HTTP_ERROR(ret = http_readline(sockfd, s, &len))) {
		return (ret);
	}

	char *msg, *ver;
	int code;

	/* parse reply */
	if (HTTP_ERROR(ret = http_reply_parse(s, &ver, &code, &msg))) {
		return (ret);
	}

	if (reply_cb && reply_cb(ver, code, msg, priv)) {
		return (HTTP_ABORTED);
	}

	char *header, *value;

	/* parse headers */
	for (;;) {
		len = sizeof(s);
		if (HTTP_ERROR(ret = http_readline(sockfd, s, &len))) {
			return (ret);
		}

		if (!len) {
			break;
		}

		if (HTTP_ERROR(ret = http_header_parse(s, &header, &value))) {
			return (ret);
		}

		if (header_cb && header_cb(header, value, priv)) {
			return (HTTP_ABORTED);
		}
	}

	return (HTTP_SUCCESS);
}
