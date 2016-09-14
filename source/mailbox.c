#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "libtools/mailbox.h"
#include "libtools/tools.h"

/*------------------------------------------------------------------------*/

struct mailbox {
	/** mutex for thread-safe access to mailbox*/
	pthread_mutex_t mutex;

	/** resource semaphore of free slots for messages */
	sem_t empty;

	/** resource semaphore of messages */
	sem_t full;

	/** head pointer of mailbox */
	unsigned head;

	/** tail pointer of mailbox */
	unsigned tail;

	/** length of mailbox */
	unsigned len;

	/** messages of mailbox */
	void *data[];
};

/*------------------------------------------------------------------------*/

mailbox_t mailbox_alloc(mailbox_t *mb, unsigned len)
{
	assert(mb);
	assert(len);

	int rc;

	if (!(*mb = malloc(sizeof(**mb) + sizeof((*mb)->data[0]) * len))) {
		return (*mb);
	}

	if ((rc = pthread_mutex_init(&(*mb)->mutex, NULL))) {
		/* update errno to indicate error */
		errno = rc;

		goto mutex_err;
	}

	if (sem_init(&(*mb)->empty, 0, len)) {
		goto empty_err;
	}

	if (sem_init(&(*mb)->full, 0, 0)) {
		goto full_err;
	}

	(*mb)->head = 0;
	(*mb)->tail = 0;
	(*mb)->len = len;

	return (*mb);

	/* handle errors */
full_err:
	rc = sem_destroy(&(*mb)->empty);
	assert(!rc);

empty_err:
	rc = pthread_mutex_destroy(&(*mb)->mutex);
	assert(!rc);

mutex_err:
	free(*mb);

	return (*mb = NULL);
}

/*------------------------------------------------------------------------*/

void mailbox_destroy(mailbox_t mb)
{
	if (!mb) {
		return;
	}

	int rc;

	unused(rc);

	rc = pthread_mutex_destroy(&mb->mutex);
	assert(!rc);

	rc = sem_destroy(&mb->full);
	assert(!rc);

	rc = sem_destroy(&mb->empty);
	assert(!rc);

	free(mb);
}

/*------------------------------------------------------------------------*/

int mailbox_post(mailbox_t mb, void *data)
{
	assert(mb);

	int rc;

	unused(rc);

	if (sem_wait(&mb->empty)) {
		return (-1);
	}

	rc = pthread_mutex_lock(&mb->mutex);
	assert(!rc);

	mb->data[mb->head] = data;
	++ mb->head;

	if (mb->head == mb->len) {
		mb->head = 0;
	}

	rc = sem_post(&mb->full);
	assert(!rc);

	rc = pthread_mutex_unlock(&mb->mutex);
	assert(!rc);

	return (0);
}

/*------------------------------------------------------------------------*/

int mailbox_wait(mailbox_t mb, void **data)
{
	assert(mb);
	assert(data);

	int rc;

	unused(rc);

	if (sem_wait(&mb->full)) {
		return (-1);
	}

	rc = pthread_mutex_lock(&mb->mutex);
	assert(!rc);

	*data = mb->data[mb->tail];
	++ mb->tail;

	if (mb->tail == mb->len) {
		mb->tail = 0;
	}

	rc = sem_post(&mb->empty);
	assert(!rc);

	rc = pthread_mutex_unlock(&mb->mutex);
	assert(!rc);

	return (0);
}

/*------------------------------------------------------------------------*/

int mailbox_timedwait(mailbox_t mb, void **data, unsigned timeout)
{
	assert(mb);
	assert(data);

	struct timespec ts;
	int rc;

	unused(rc);

	rc = clock_gettime(CLOCK_REALTIME, &ts);
	assert(!rc);

	ts.tv_sec += timeout;

	if (sem_timedwait(&mb->full, &ts)) {
		return (-1);
	}

	rc = pthread_mutex_lock(&mb->mutex);
	assert(!rc);

	*data = mb->data[mb->tail];
	++ mb->tail;

	if (mb->tail == mb->len) {
		mb->tail = 0;
	}

	rc = sem_post(&mb->empty);
	assert(!rc);

	rc = pthread_mutex_unlock(&mb->mutex);
	assert(!rc);

	return (0);
}
