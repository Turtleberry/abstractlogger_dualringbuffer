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
					errflg = 1;
			}
			if ((rethdr != NULL) && (errflg == 1))
			{
				/* TODO: Loveliness abounds partially allocated structure needs to be freed */
			}
		}
	}
	return rethdr;
}

RbData * createRingBufferData(const rb_data_flags rbdflags, const size_t buffsize, const int numelements)
{
	/* Since this is not a double linked list, we're going to create an array to store all of the ptrs to allocated elements temporarily
	 * such that we can very easily free everything without having to iterate through the single linked list.
	 * This idea may be flogic... Going to think about it.
	 */

}

RingBuff * createRingBuff(const rb_config_flags rbcflags, const rb_data_flags rbdflags, const int numelements, const size_t dataSize)
{
	RingBuff *retval;
	/* TODO: Right now, we'll ignore config flags 'til this is tested */
	retval = (RingBuff *) malloc(sizeof(RingBuff));
	if (retval != NULL)
	{
		if (pthread_mutex_init(retval->rb_mutex, NULL) == 0)
		{
			/* Blew chow */
			free(retval);
			retval = NULL;
		}
		else
		{

		}
	}
	return retval;
}

int createRingBuffer(RbHeader *rb_header, const rb_config_flags flags, const int numelements)
{
	int create_status = RB_SUCCESS;
	rb_config_flags rbcflags;
	rb_data_flags rbdflags;
	rbcflags.all_flags = 0;
	rbdflags.all_flags = 0;
	for (int i=0; ((i < 2) && (create_status == RB_SUCCESS)); i++)
	{
		if ((rb_header->ringbufferarray[i] = createRingBuff(rbcflags, rbdflags, numelements, rb_header->dsize)) != NULL)
		{
			printf("Ringbuffer %i created\n", i);
		}
		else
		{
			create_status = RB_FAILURE;
			break;
		}
	}
	return create_status;
}



/* For testing purposes, I'm creating main here. Once this code is done, I'll put this to the appropriate test files or framework */
int main (int argc, char **argv)
{
	RbHeader *myRbHeader = NULL;
	const size_t rbdatasize = 256;
	const int numElements = 100;

	printf("RingBuffer test\n");
	printf("Creating RingBuffer header\n");
	rb_hdr_flags myFlags;
	myFlags.all_flags = 0U;

	myRbHeader = createRingBufferHeader(myFlags, (size_t) rbdatasize, 0); /* Header with 256 byte data element size */
	if (myRbHeader != NULL)
	{
		printf("RingBuffer header created. Creating elements\n");
		rb_config_flags my_config_flags;
		my_config_flags.all_flags = 0;
		if (createRingBuffer(myRbHeader, my_config_flags, numElements) == RB_SUCCESS)
		{
			printf("Dual ring buffer created\n");
			/* TODO: Create test threads, benchmark */
		}
	}
	else
		printf("Memory allocation failure attempting to create header\n");

	return 0;
}






