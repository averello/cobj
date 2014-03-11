//
//  testValue.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <cobj.h>
#include <string.h>

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

int main2() {
	puts("hello world");
	return 0;
}

void cleanup(void *pointer) {
	free(pointer);
}

int main () {
	typedef int ( *mainFunction) ();
	/* Test creation and getter */
	{
		mainFunction mainP = main;
		ValueRef v1 = new(Value, mainP, NULL);
		assert( v1 != NULL);
		assert( getValuePointer(v1) != NULL);
		assert( ((mainFunction)getValuePointer(v1)) == mainP );
		release(v1);
	}
	/* Test setter */
	{
		mainFunction mainP = main;
		int *asjfdhdsjhf = calloc(5, sizeof(int));
		ValueRef v1 = new(Value, mainP, NULL);
		setValuePointer(v1, asjfdhdsjhf);
		assert( getValuePointer(v1) != NULL );
		assert( getValuePointer(v1) == asjfdhdsjhf);
		
		free(asjfdhdsjhf);
		release(v1);
	}
	
	/* Test failed creation */
	{
		ValueRef v1 = new(Value, NULL);
		assert( v1 == NULL );
	}
	
	{
		mainFunction mainP = main;
		ValueRef v1 = new(Value, mainP, NULL);
		ValueRef v2 = new(Value, mainP, NULL);
		
		UInteger h1 = hash(v1);
		UInteger h2 = hash(v2);
		
		assert( h1 == h2 );
		
		const mainFunction main2P = main2;
		setValuePointer(v2, (const void *)main2P);
		
		h1 = hash(v1);
		h2 = hash(v2);
		
		assert( h1 != h2 );
		
		release(v1), release(v2);
	}
	
	/* Test cleanup function */
	{
		void *a = strdup("Hello World");
		ValueRef v1 = new(Value, a, cleanup, NULL);
		
		release(v1);
	}
	return 0;
}
