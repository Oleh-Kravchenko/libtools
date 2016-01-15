/**
 * @file
 *
 * Copyright (C) 2016  Oleh Kravchenko <oleg@kaa.org.ua>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TOOLS_HTTP_H
#define __TOOLS_HTTP_H

#include <stddef.h>

/**
 * @addtogroup network
 *
 * @{
 */

/*------------------------------------------------------------------------*/

/** HTTP methods, see rfc2616 */
typedef enum http_method {
	HTTP_OPTIONS,
	HTTP_GET,
	HTTP_HEAD,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE,
	HTTP_TRACE,
	HTTP_CONNECT,
} http_method_t;

/*------------------------------------------------------------------------*/

/** HTTP status code */
typedef enum http_status {
	HTTP_SUCCESS           = 0,
	HTTP_CONNECTION_CLOSED,
	HTTP_SOCKET_ERROR,
	HTTP_LARGE_REQUEST,
	HTTP_INVALID_REQUEST,
	HTTP_UNKNOWN_REQUEST,
	HTTP_INVALID_HEADER,
	HTTP_INVALID_REPLY,
	HTTP_ABORTED,
	HTTP_OUT_MEMORY,
} http_status_t;

/*------------------------------------------------------------------------*/

/** HTTP header values type */
typedef enum http_header_type {
	HTTP_HEADER_STRING,
	HTTP_HEADER_INT,
} http_header_type_t;

/** HTTP header type */
typedef struct http_header {
	/** name of header */
	char *name;

	/** type of header */
	http_header_type_t type;

	/** header value */
	union {
		/** value is a string */
		char *s;

		/** value is a integer */
		int i;
	} val;
} http_header_t;

/**
 * @brief define HTTP header name with string value
 * @param [in] n HTTP header name
 * @param [in] v HTTP header value
 */
#define HTTP_HEADER_STRING(n, v)                                          \
	{ .type = HTTP_HEADER_STRING, .name = (n), .val.s = (v), }

/**
 * @brief define HTTP header name with integer value
 * @param [in] n HTTP header name
 * @param [in] v HTTP header value
 */
#define HTTP_HEADER_INT(n, v)                                             \
	{ .type = HTTP_HEADER_INT, .name = (n), .val.i = (v), }

/*------------------------------------------------------------------------*/

/**
 * @brief return true if HTTP error occurred
 * @param [in] x HTTP response value
 * @return comparison result
 */
#define HTTP_ERROR(x) ((x) != HTTP_SUCCESS)

/*------------------------------------------------------------------------*/

/**
 * @brief pointer to a callback function to process request
 * @param [in] method HTTP method
 * @param [in] path resource path
 * @param [in] ver version of HTTP protocol
 * @param [in] priv private data for callback
 * @return result of the function
 * @retval 0 if function ran properly, other values will mean an error
 */
typedef int (*http_request_cb_t)(http_method_t m, const char *path, const char *ver, void *priv);

/**
 * @brief pointer to a callback function to process reply
 * @param [in] ver version of HTTP protocol
 * @param [in] code HTTP reply code
 * @param [in] msg HTTP message
 * @param [in] priv private data for callback
 * @return result of the function
 * @retval 0 if function ran properly, other values will mean an error
 */
typedef int (*http_reply_cb_t)(const char *ver, int code, const char *msg, void *priv);

/**
 * @brief pointer to a callback function to process header
 * @param [in] name HTTP header name
 * @param [in] value HTTP header value
 * @param [in] priv private data for callback
 * @return result of the function
 * @retval 0 if function ran properly, other values will mean an error
 */
typedef int (*http_header_cb_t)(const char *name, const char *value, void *priv);

/*------------------------------------------------------------------------*/

/**
 * @brief send HTTP request by socket
 * @param [in] sockfd socket descriptor
 * @param [in] method HTTP method
 * @param [in] path resource path
 * @param [in] ver version of HTTP protocol
 * @param [in] hdr array of headers
 * @param [in] count count of headers in @p hdr
 * @return HTTP status code
 * @retval HTTP_UNKNOWN_REQUEST unknown HTTP request
 * @retval HTTP_INVALID_REQUEST can't create valid HTTP request
 * @retval HTTP_OUT_MEMORY can't complete HTTP request
 */
http_status_t http_request_send(int sockfd, http_method_t method, const char *path, const char *ver, http_header_t hdr[], size_t count);

/**
 * @brief read HTTP request from socket and parse it
 * @param [in] sockfd socket descriptor
 * @param [in] request_cb request callback
 * @param [in] header_cb header callback
 * @param [in] priv private data for callbacks
 * @return HTTP status code
 * @retval HTTP_ABORTED aborted by callback
 */
http_status_t http_request_recv(int sockfd, http_request_cb_t request_cb, http_header_cb_t header_cb, void *priv);

/**
 * @brief send HTTP reply to socket
 * @param [in] sockfd socket descriptor
 * @param [in] ver HTTP version
 * @param [in] code HTTP code
 * @param [in] desc HTTP reply description
 * @param [in] hdr headers
 * @param [in] count headers count
 * @return HTTP status code
 * @retval HTTP_INVALID_REQUEST invalid version
 * @retval HTTP_OUT_MEMORY out of memory to reply
 */
http_status_t http_reply_send(int sockfd, const char *ver, int code, const char *desc, http_header_t hdr[], size_t count);

/**
 * @brief read HTTP reply from socket and parse it
 * @param [in] sockfd socket descriptor
 * @param reply_cb reply callback
 * @param header_cb header callback
 * @param priv private data for callbacks
 * @return HTTP status code
 * @retval HTTP_ABORTED aborted by callback
 * @retval HTTP_INVALID_REPLY invalid HTTP reply
 */
http_status_t http_reply_recv(int sockfd, http_reply_cb_t reply_cb, http_header_cb_t header_cb, void *priv);

/** @} */

#endif /* __TOOLS_HTTP_H */
