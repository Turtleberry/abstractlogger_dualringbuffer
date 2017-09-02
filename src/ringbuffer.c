/*
 * ringbuffer.c
 *
 *  Created on: Sep 1, 2017
 *      Author: dhenders
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ringbuffer.h>

/***
 * createRingBufferHeader allocates memory for the ring buffer header, and returns the created structure on success or NULL on failure.
 */
RbHeader * createRingBufferHeader(rb_hdr_flags header_flags, size_t datasize, int numelements)
{
	RbHeader * rethdr;
	int errflg = 0;
	rethdr = (RbHeader *) malloc(sizeof(RbHeader));

	if (rethdr != NULL)
	{
		if ((header_flags.use_shm) && (numelements == 0))
		{
			/* We can't allocate the shared memory for this, so we're kinda toast */
			/* Mind you, I could change the implementation so that it could allocate it separately, but for this purpose, I don't want to */
			printf("Shared memory usage specified, and numelements is zero.\n");
			free(rethdr);
			rethdr = NULL;
		}
		else
		{
			rethdr->dsize = datasize;
			rethdr->flags = header_flags;
			/* Create and initialize mutexes */
			if (header_flags.use_shm)
			{
				/* We need to allocate enough shared memory to store this structure, and everything else associated with this ring buffer */
				/* To simplify that process, I'm writing a set of functions for that purpose, because the shm block will be allocated, and then */
				/* everything within the ring buffer will be allocated from that pool. */
			}
			else
			{
				if (pthread_mutex_init(rethdr->rb_mutex_write, NULL) == 0)
				{
					if (header_flags.multi_read)
					{
						if (pthread_mutex_init(rethdr->rb_mutex_read, NULL) != 0)
						{
							printf("Error initalizing rb_mutex_read.\n");
							errflg = 1;

						}
					}
					else
						rethdr->rb_mutex_read = NULL;
				}
				else
					errflg = 1;
			}
			if ((rethdr != NULL && (errflg == 1))
			{
				/* Loveliness abounds partially allocated structure needs to be freed */
			}
		}
	}
	return rethdr;
}


/* For testing purposes, I'm creating main here. Once this code is done, I'll put this to the appropriate test files or framework */
int main (int argc, char **argv)
{
	RbHeader *myHeader = NULL;

	printf("RingBuffer test\n");
	printf("Creating RingBuffer header");
	rb_hdr_flags myFlags;
	myFlags.all_flags = 0U;

	myHeader = createRingBufferHeader(myFlags, (size_t) 256, 0); /* Header with 256 byte data element size */
	if (myHeader != NULL)
	{
		printf("RingBuffer header created. Creating elements");

	}
	else
		printf("Memory allocation failure attempting to create header\n");

	return 0;
}






