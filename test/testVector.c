//
//  testVector.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <errno.h>

extern int errno;

#include <cobj.h>

int main () {
	/* Testing creation */
	{
		VectorRef vector = new(Vector, 10, 10, NULL);
		assert( vector != NULL );
		assert( getVectorCapacity(vector) == 10 );
		assert( getVectorCapacityIncrement(vector) == 10 );
		
		assert( getObjectAtIndex(vector, 0) == NULL );
		assert( getObjectAtIndex(vector, 5) == NULL && errno == EINVAL);
		assert( firstObject(vector) == NULL );
		assert( lastObject(vector) == NULL );
		errno = 0;
		removeObjectAtIndex(vector, 0);
		assert( errno == EINVAL );
		
		errno = 0;
		removeObjectAtIndex(vector, 1);
		assert( errno == EINVAL );
		
		assert( indexOfObject(vector, vector) == ANotFound );
		assert( ! arrayContainsObject(vector, vector) );
		
		release(vector);
	}
	
	/* Testing setters and getters */
	{
		StringRef s1 = new(String, "string 1", NULL);
		StringRef s2 = new(String, "string 2", NULL);
		StringRef s3 = new(String, "string 3", NULL);
		StringRef s4 = new(String, "string 4", NULL);
		VectorRef vector = new(Vector, 10, 10, NULL);

		addObject(vector, s1);
		
		assert( getCollectionCount(vector) == 1);

		assert( getObjectAtIndex(vector, 0) == s1 );
		assert( getObjectAtIndex(vector, 5) == NULL && errno == EINVAL);
		assert( firstObject(vector) == s1 );
		assert( lastObject(vector) == s1 );
		
		unsigned long indexOfS1 = indexOfObject(vector, s1);
		assert( indexOfS1 != ANotFound );
		assert( indexOfS1 == 0 );
		assert( arrayContainsObject(vector, s1) );
		
		errno = 0;
		removeObjectAtIndex(vector, 0);
		assert( errno == 0 );
		
		addObject(vector, s1);
		removeAllObjects(vector);
		assert( getCollectionCount(vector) == 0 );
		
		addObject(vector, s1);
		removeFirstObject(vector);
		assert( indexOfObject(vector, s1) == ANotFound );
		
		addObject(vector, s1);
		addObject(vector, s2);
		addObject(vector, s1);
		addObject(vector, s2);
		
		assert( getCollectionCount(vector) == 4 );
		assert( lastObject(vector) == s2 );
		assert( firstObject(vector) == s1 );
		
		errno = 0;
		removeObjectAtIndex(vector, 1);
		assert( errno != EINVAL );
		assert( getObjectAtIndex(vector, 1) == s1 );
		
		insertObject(vector, s3);
		assert( firstObject(vector) == s3 );
		
		removeObjectAtIndex(vector, 1);
		insertObjectAtIndex(vector, s4, 3);
		assert( getObjectAtIndex(vector, 3) == s4 );
		
		release(s1), release(s2), release(s3), release(s4);
		release(vector);
	}
	
	/* Testing massive operations and capacity changes */
	{
		VectorRef vector = new(Vector, 10, 10, NULL);
		for (int i=0; i<100; i++) {
			StringRef string = newStringWithFormat(String, "string %d", i);
			addObject(vector, string);
			release(string);
		}
		assert( getVectorCapacity(vector) == 100 );
		assert( getCollectionCount(vector) == 100 );
		
		removeAllObjects(vector);
		assert( getCollectionCount(vector) == 0 );
		
		
		for (int i=0; i<100; i++) {
			StringRef string = newStringWithFormat(String, "string %d", i);
			addObject(vector, string);
			release(string);
		}
		assert( setVectorSize(vector, 15) == 0 );
		assert( getCollectionCount(vector) == 15 );
		
		
		release(vector);
	}
	
	/* Testing massive operations and capacity changes */
	{
		VectorRef vector = new(Vector, 10, 10, NULL);
		setVectorCapacityIncrement(vector, 50);
		for (int i=0; i<100; i++) {
			StringRef string = newStringWithFormat(String, "string %d", i);
			addObject(vector, string);
			release(string);
		}
		assert( getVectorCapacity(vector) == 110 );
		assert( getCollectionCount(vector) == 100 );
		
		release(vector);
	}
	
	/* Testing auto add */
	{
		VectorRef vector = new(Vector, 10, 10, NULL);
		addObject(vector, vector);
		release(vector);
	}
	
	{/* Testing removeObjectsInRange() */
		VectorRef vector = new(Vector, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef string = newStringWithFormat(String, "String %d", i, NULL);
			addObject(vector, string);
			release(string);
		}
		
		SRange range = SMakeRange(5, 4);
		removeObjectsInRange(vector, range);
		
		assert( getCollectionCount(vector) == 6 );
		assert( strcmp("String 9", getStringText(getObjectAtIndex(vector, 5))) == 0 );
		
		release(vector);
	}
	
	{/* Testing replaceObjectAtIndex */
		VectorRef vector = new(Vector, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef string = newStringWithFormat(String, "String %d", i, NULL);
			addObject(vector, string);
			release(string);
		}
		
		StringRef string = new(String, "String 42", NULL);
		replaceObjectAtIndexWithObject(vector, 5, string);
		release(string);
		
		assert( getCollectionCount(vector) == 10 );
		assert( strcmp("String 42", getStringText(getObjectAtIndex(vector, 5))) == 0 );
		
		release(vector);
	}
	
	return 0;
}

