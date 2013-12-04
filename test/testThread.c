//
//  testThread.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <unistd.h>

#include <cobj.h>

void * threadFunction(void *args);
void * threadFunctionArgument(void *args);

int main () {
	/* Test creation */
	{
		ThreadRef thread = new(Thread, threadFunction, NULL);
		assert( thread != NULL );
		release(thread);
	}
	
	/* Test start and join */
	{
		ThreadRef thread = new(Thread, threadFunction, NULL);
		startThread(thread);
		joinThread(thread, NULL);
		puts("- joined");
		release(thread);
	}
	
	/* Test arguments */
	{
		StringRef threadString = new(String, "StringRef for thread", NULL);
		ThreadRef thread = new(Thread, threadFunctionArgument, threadString, NULL);
		startThread(thread);
		joinThread(thread, NULL);
		puts("- joined");
		release(thread);
		
		
		StringRef outString;
		thread = new(Thread, threadFunctionArgument, threadString, NULL);
		startThread(thread);
		joinThread(thread, &outString);
		assert( outString != NULL );
		assert( outString == threadString );
		puts("- joined");
		release(threadString);
		release(thread);
	}
	return 0;
}

void * threadFunction(void *args) {
	printf("Hello i am thread %ld\n", (long)pthread_self());
	sleep(1);
	pthread_exit(NULL);
}

void * threadFunctionArgument(void *args) {
	StringRef string = retain(args);
	puts(getStringText(string));
	release(string);
	pthread_exit(string);
}
