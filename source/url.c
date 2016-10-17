#include <assert.h>
#include <string.h>
#include <sys/un.h>
#include <uriparser/Uri.h>

#include "libtools/gethostaddr.h"
#include "libtools/string.h"
#include "libtools/url.h"

/*------------------------------------------------------------------------*/

/**
 * @brief Extract parsed URI components
 * @param [in] uri pointer to parser URI
 * @param [in,out] u pointer to URL components
 * @return on success, zero is returned
 * @retval -1 error occurred
 */
static inline int url_parse_internal(UriUriA *uri, struct url *u)
{
	/* extract scheme */
	if (uri->scheme.first && !str_ncat(
		&u->scheme,
		uri->scheme.first,
		uri->scheme.afterLast - uri->scheme.first
	)) {
		return (-1);
	}

	/* extract username and password */
	if (uri->userInfo.first) {
		char *userinfo = NULL;

		if (!str_ncat(
			&userinfo,
			uri->userInfo.first,
			uri->userInfo.afterLast - uri->userInfo.first
		)){
			return (-1);
		}

		char *password = strchr(userinfo, ':');

		if (password) {
			/* split userinfo to username and password */
			*password ++ = '\0';

			u->password = strdup(password);
		}

		u->username = strdup(userinfo);
		free(userinfo);

		/* verify memory allocation for username and password */
		if (!u->username || (password && !u->password)) {
			return (-1);
		}
	}

	/* extract hostname */
	if (uri->hostText.first && !str_ncat(
		&u->hostname,
		uri->hostText.first,
		uri->hostText.afterLast - uri->hostText.first
	)) {
		return (-1);
	}

	/* extract port number */
	if (uri->portText.first) {
		u->port = atoi(uri->portText.first);
	}

	/* extract path*/
	if (uri->pathHead && uri->pathTail && uri->pathHead->text.first) {
		if (uri->absolutePath && !(u->path = strdup("/"))) {
			return (-1);
		}

		if (!str_ncat(&u->path,
			uri->pathHead->text.first,
			uri->pathTail->text.afterLast - uri->pathHead->text.first
		)) {
			return (-1);
		}
	}

	/* extract query */
	if (uri->query.first && !str_ncat(
		&u->query,
		uri->query.first,
		uri->query.afterLast - uri->query.first
	)) {
		return (-1);
	}

	/* extract fragment */
	if (uri->fragment.first && !str_ncat(
		&u->fragment,
		uri->fragment.first,
		uri->fragment.afterLast - uri->fragment.first
	)) {
		return (-1);
	}

	return (0);
}

/*------------------------------------------------------------------------*/

struct url *url_parse(const char *s, struct url **u)
{
	assert(s);
	assert(u);

	if (!(*u = calloc(1, sizeof(**u)))) {
		return (*u);
	}

	int error = -1;
	UriUriA uri;

	/* parse URI by uriparser library */
	if (uriParseUriA(&(UriParserStateA){.uri = &uri}, s) == URI_SUCCESS) {
		/* try to extract parsed URI */
		error = url_parse_internal(&uri, *u);

		/* allow to specify path in hostname with %2f delimiter */
		uriUnescapeInPlaceA((*u)->hostname);
	}

	uriFreeUriMembersA(&uri);

	/* if extraction failed, free allocated memory */
	if (error) {
		url_free(*u);
		*u = NULL;
	}

	return (*u);
}

/*------------------------------------------------------------------------*/

void url_free(struct url *u)
{
	if (u) {
		free(u->scheme);
		free(u->username);
		free(u->password);
		free(u->hostname);
		free(u->path);
		free(u->query);
		free(u->fragment);
		free(u);
	}
}

/*------------------------------------------------------------------------*/

int url2sockaddr(struct url *u, struct sockaddr_storage *sa)
{
	memset(sa, 0, sizeof(*sa));

	/* socket address for HTTP */
	if (u->scheme && u->hostname && !strcmp(u->scheme, "http")) {
		struct sockaddr_in *in = (struct sockaddr_in*)sa;

		in->sin_family = AF_INET;
		in->sin_port = htons(u->port ? u->port : 80);

		/* resolving hostname */
		if (!gethostaddr(u->hostname, &in->sin_addr)) {
			return (-1);
		}
	/* socket address for UNIX */
	} else if (u->scheme && u->hostname && (
		!strcmp(u->scheme, "local") ||
		!strcmp(u->scheme, "unix")
	)) {
		struct sockaddr_un *un = (struct sockaddr_un*)sa;

		un->sun_family = AF_LOCAL;
		strlcpy(un->sun_path, u->hostname, sizeof(un->sun_path));
	} else {
		return (-1);
	}

	return (0);
}

/*------------------------------------------------------------------------*/

socklen_t sockaddr_size(const struct sockaddr_storage *sa)
{
	switch (sa->ss_family) {
		case AF_UNIX:
			return (sizeof(struct sockaddr_un));

		case AF_INET:
			return (sizeof(struct sockaddr_in));

		case AF_INET6:
			return (sizeof(struct sockaddr_in6));

		default:
			return (sizeof(struct sockaddr));
	}
}
