# abstractlogger_dualringbuffer
Implementing an abstract logger, but adding a semi-unique twist with an interleaved ring buffer.
Note: I have a massive headache, and am running on about 2 hours of sleep. I'm not sure how
good this code is gonna be, but I'll get crackin' on it.

Correction: I am going to bed, as the screen is now blurry.

** Back from the migraine ** 
So, what this is:
A] Simple logger utilizing a vtable for an abstracted interface
B] This logger utilizes a dual ring buffer with statically allocated memory. The reason for using
		dual ring buffers, is so that multiple-write/single read latencies can (at least its my belief)
		be minimized.
C] Proof of concept code

A dual ring buffer... why? Is two really better than one? I don't know, but I'll find out shortly. Here's
my thought process:
		Given that you have multiple processes writing to a single logging "pipe," placing the logged
		messages into a buffer for future processing makes sense. I chose a ring buffer with statically
		allocated elements, so that I can simply memcpy the log structure that has been passed in from the
		logging interface.
		
		The potential issue that I see, is that with a single ring buffer, ultimately the writing
		processes are gated by the single reading process. My theory, is that by having two ring
		buffers that are written to, the writers are less likely to be gated by the reading/processing
		process.
		
		My initial plan is to utilize semaphores instead of mutexes, but this may change.
		

		
		