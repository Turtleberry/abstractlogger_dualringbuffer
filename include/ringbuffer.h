/*
 * ringbuffer.h
 *
 *  Created on: Sep 1, 2017
 *      Author: dhenders
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>
#include <pthread.h>


/* The following may seem odd, but I'm using bitfields and a union for the flags */

typedef union
{
	uint8_t all_flags;
	struct {
		uint8_t use_shm :1,     /* Use shared memory */
				multi_read : 1, /* Support multiple readers */
				spare5 : 1,     /* Unused */
				spare4 : 1,     /* Unused */
				spare3 : 1,     /* Unused */
				spare2 : 1,     /* Unused */
				spare1 : 1,     /* Unused */
				spare0 : 1;     /* Unused */
	};
} rb_ctl_flags;

typedef union
{
	uint8_t all_flags;
	struct {
		uint8_t multi_read : 1,            /* Support multiple readers */
				broadcast_readers : 1,     /* If using multiple readers, broadcast or single thread */
				use_shm : 1,               /* Use shared memory */
				spare4 : 1,     /* Unused */
				spare3 : 1,     /* Unused */
				spare2 : 1,     /* Unused */
				spare1 : 1,     /* Unused */
				spare0 : 1;     /* Unused */
	};
} rb_hdr_flags;

typedef union
{
	uint8_t all_flags;
	struct {
		uint8_t collect_stats : 1,     /* Collect statistics */
				spare6 : 1,     /* Unused */
				spare5 : 1,     /* Unused */
				spare4 : 1,     /* Unused */
				spare3 : 1,     /* Unused */
				spare2 : 1,     /* Unused */
				spare1 : 1,     /* Unused */
				spare0 : 1;     /* Unused */
	};
} rb_config_flags;

typedef union
{
	uint8_t all_flags;
	struct {
		uint8_t spare7 : 1,     /* Unused */
				spare6 : 1,     /* Unused */
				spare5 : 1,     /* Unused */
				spare4 : 1,     /* Unused */
				spare3 : 1,     /* Unused */
				spare2 : 1,     /* Unused */
				spare1 : 1,     /* Unused */
				spare0 : 1;     /* Unused */
	};
} rb_status_flags;

typedef union
{
	uint8_t all_flags;
	struct {
		uint8_t spare7 : 1,     /* Unused */
				spare6 : 1,     /* Unused */
				spare5 : 1,     /* Unused */
				spare4 : 1,     /* Unused */
				spare3 : 1,     /* Unused */
				spare2 : 1,     /* Unused */
				spare1 : 1,     /* Unused */
				spare0 : 1;     /* Unused */
	};
} rb_data_flags;

struct ringbuff_ctl {
	rb_ctl_flags flags; /* Operational control and mgmt flags... are we supporting shared memory, or is this for single processes only */

};


typedef struct rbdata {
	rb_data_flags flags; /* Data level flags */
	void *dataptr;  /* Ring buffer data */
	void *next;  /* Single linked list. This points to next... end points back to head */
} RbData;

typedef struct  {
	long curwrite; /* Current number of writes */
	long curread;  /* Current number of reads */
	long maxdiff;  /* Max diff between write and read */
} rb_stats;

typedef struct ringbuff {
	pthread_mutex_t *rb_mutex; /* Used to insure that this rb is only accessed by a single reader or writer */
	rb_config_flags configflags; /* Ringbuffer configuration flags */
	rb_status_flags statusflags; /* Ringbuffer status flags */
	rb_stats statistics;
	RbData *curreadptr;
	RbData *curwriteptr;
} RingBuff;

typedef struct ringbuff_hdr {
	size_t dsize; /* Data size of ring buffer data block*/
	rb_hdr_flags flags; /* Flags for all ring buffers, and anything with this functionality */
	pthread_mutex_t *rb_mutex_write; /* Writes require a mutex, whereas the reader does not, since there is only a single reader */
	pthread_mutex_t *rb_mutex_read; /* Just in case someone wants to use multiple readers and totally fubar my original design premise */
	int writerbindx; /* index of currently active write ring buffer 0 or 1 */
	int readerbindx; /* index of currently active read ring buffer 0 or 1 */
	RingBuff *ringbufferarray[2];
} RbHeader;


/* Function protos */
RbHeader * createRingBufferHeader(rb_hdr_flags header_flags, size_t dataSize, int numElements); /* Create rb header, set data block size */
int createRingBuffer(RbHeader *rb_header, rb_config_flags flags, int numElements); /* Creates ring buffer and all of its elements */
RbData *createRingBufferDataElement(rb_data_flags flags, size_t dataSize); /* Creates rb data element */
RbData *getRingBufferData(RbData *curRead);


#endif /* RINGBUFFER_H_ */
