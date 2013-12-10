/*!
 *  @file testConcurrentMutableArray.c
 *  @brief CObjects
 *
 *  Created by @author George Boumis
 *  @date 27/4/13.
 *  @copyright   Copyright (c) 2013 George Boumis. All rights reserved.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cobj.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

typedef void (*voidf)(void);

struct _args {
	ConcurrentMutableArrayRef array;
	voidf function;
} Args;

static void * threadFunction(void *args);

int main () {
	{	/* Testing allocation and waiting getObject with an addObject */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		assert( array != NULL );
		Args.array = array;
		Args.function = (voidf)addObject;
		
		ThreadRef thread = new(Thread, threadFunction, array, NULL);
		startThread(thread);
		
		StringRef s1 = getObjectAtIndex(array, 0);
		assert( s1 != NULL );
		assert( getStringLength(s1) == 8 );
		assert( strcmp(getStringText(s1), "string 1") == 0 );
		
		joinThread(thread, NULL);
		
		
		release(array),
		release(s1),
		release(thread);
	}
	
	{	/* Testing allocation and waiting getObject with insertObject */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		ThreadRef thread = new(Thread, threadFunction, array, NULL);
		Args.array = array;
		Args.function = (voidf)insertObject;
		
		startThread(thread);
		
		StringRef s1 = getObjectAtIndex(array, 0);
		assert( s1 != NULL );
		assert( getStringLength(s1) == 8 );
		assert( strcmp(getStringText(s1), "string 1") == 0 );
		
		joinThread(thread, NULL);
		
		
		release(array),
		release(s1),
		release(thread);
	}
	
	{	/* insertObjectAtIndex */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		ThreadRef thread = new(Thread, threadFunction, array, NULL);
		Args.array = array;
		Args.function = (voidf)insertObjectAtIndex;
		
		for (int i=0; i<10; i++) {
			StringRef s = newStringWithFormat(String, "string %2d", i, NULL);
			insertObject(array, s);
			release(s);
		}
		
		startThread(thread);
		
		sleep(1);
		int i=0;
		while(i++<1000*4);
		StringRef s1 = getObjectAtIndex(array, 5);
		assert( s1 != NULL );
		assert( getStringLength(s1) == 5 || getStringLength(s1) == 9 );
		assert( strcmp(getStringText(s1), "joker") == 0 || strcmp(getStringText(s1), "string  6"));
		
		joinThread(thread, NULL);
		
		
		release(array),
		release(thread);
	}
	
	{	/* insertObjectAtIndex */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef s = newStringWithFormat(String, "string %2d", i, NULL);
			insertObject(array, s);
			release(s);
		}
		
		ConcurrentMutableArrayRef cpy = copy(array);
		assert( cpy != NULL );
		assert( getCollectionCount(cpy) == getCollectionCount(array) );
		
		release(array), release(cpy);
	}
	
	{	/* insertObjectAtIndex */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef s = newStringWithFormat(String, "string %2d", i, NULL);
			insertObject(array, s);
			release(s);
		}
		removeObjectAtIndex(array, 0);
		assert( getCollectionCount(array) == 9 );
		
		release(array);
	}
	
	{	/* popObject */
		ConcurrentMutableArrayRef array = new(ConcurrentMutableArray, NULL);
		
		int size = 10;
		for (int i=0; i<size; i++) {
			StringRef s = newStringWithFormat(String, "string %2d", i, NULL);
			insertObject(array, s);
			release(s);
		}
		
		for (;size>0;size--) {
			StringRef s = popObject(array);
			assert( s != NULL );
			assert( getCollectionCount(array) == size-1 );
		}
		assert( getCollectionCount(array) == 0 );
		
		release(array);
	}
	
	return 0;
}

static void * threadFunction(void *args) {
	ConcurrentMutableArrayRef array = Args.array;
	sleep(1);
	StringRef s1 = new(String, "string 1", NULL);
	if ( Args.function == (voidf)addObject )
		addObject(array, s1);
	else if ( Args.function == (voidf)insertObject )
		insertObject(array, s1);
	else if ( Args.function == (voidf)insertObjectAtIndex ) {
		release(s1);
		s1 = new(String, "joker", NULL);
		insertObjectAtIndex(array, s1, 5);
		release(s1);
	}
		
	return NULL;
}
