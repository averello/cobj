#include <stdio.h>
#include <stdlib.h>
#include <cobj.h>
#include <string.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

#include "minunit.h"

int tests_run = 0;
int tests_failed = 0;

int main ( int argc, char *argv[] ) {

	void * string1 = new(String, "my precious string", NULL);
	StringRef string2 = new(String, "This is 2", NULL);
	StringRef string3 = new(String, "my precious string", NULL);

	StringRef formatedString = newStringWithFormat(String, "%d %s", 0, getStringText(string1), NULL);
	
	StringRef concat = new(String, NULL);
	StringRef string4 = copy(string2);
	StringRef strings[5] = { string1, string2, string3, string4, formatedString };

	ArrayRef array = new(Array, string1, string2, string3, string4, formatedString, NULL);
	assert(array != NULL);
	assert(getCollectionCount(array) == 5);
	for (unsigned long i=0; i<getCollectionCount(array); i++)
		assert( equals(strings[i], getObjectAtIndex(array, i)) );
	
	ArrayRef copyArray = copy(array);
	assert(copyArray != NULL);
	assert(getCollectionCount(copyArray) == 5);
	assert( equals(array, copyArray) );
	
	assert( arrayContainsObject(copyArray, formatedString) );
	assert( ! arrayContainsObject(copyArray, concat) );
	
	ArrayRef lolArray = new(Array, NULL);
	assert(lolArray != NULL);
	assert(getCollectionCount(lolArray) == 0);
	assert( ! equals(copyArray, lolArray) );
	release(lolArray);
	
	assert( indexOfObject(copyArray, string2) == 1 );
	release(array);
	
//	StringRef arraycopyDescription = copyDescription(copyArray);
//	puts(getStringText(arraycopyDescription));
//	release(arraycopyDescription);

	StringRef mutableStrings[3] = { string1, string2, formatedString };
	MutableArrayRef mutable = new(MutableArray, string1, string2, formatedString, NULL);
	assert(mutable != NULL);
	assert(getCollectionCount(mutable) == 3);
	for (unsigned long i=0; i<getCollectionCount(mutable); i++)
		assert( equals(mutableStrings[i], getObjectAtIndex(mutable, i)) );
	
	MutableArrayRef mutableCopyArray = copy(mutable);
	assert( mutableCopyArray != NULL);
	assert ( equals(mutable, mutableCopyArray) );
	release(mutableCopyArray);
	release(mutable);
	
	release(formatedString);
	release(concat);
	release(string1);
	release(string2);
	release(string3);
	release(string4);
	//*/
	
	for (unsigned long i=0; i<getCollectionCount(copyArray); i++)
		assert( equals(strings[i], getObjectAtIndex(copyArray, i)) );
	release(copyArray);
		
	MutableArrayRef addRemove = new(MutableArray, NULL);
	assert( addRemove != NULL );
	StringRef s1 = new(String, "s1"), s2 = new(String, "s2"), s3 = new(String, "s3");
	addObject(addRemove, s1);
	assert( getCollectionCount(addRemove) == 1 );
	addObject(addRemove, s2);
	assert( getCollectionCount(addRemove) == 2 );
	addObject(addRemove, s3);
	assert( getCollectionCount(addRemove) == 3 );
	
	assert ( equals(s1, getObjectAtIndex(addRemove, 0)) );
	assert ( equals(s2, getObjectAtIndex(addRemove, 1)) );
	assert ( equals(s3, getObjectAtIndex(addRemove, 2)) );

	insertObject(addRemove, s1);
	assert(getCollectionCount(addRemove) == 4);
	insertObject(addRemove, s2);
	assert(getCollectionCount(addRemove) == 5);
	insertObject(addRemove, s3);
	assert(getCollectionCount(addRemove) == 6);
	
	assert ( equals(s3, getObjectAtIndex(addRemove, 0)) );
	assert ( equals(s2, getObjectAtIndex(addRemove, 1)) );
	assert ( equals(s1, getObjectAtIndex(addRemove, 2)) );
	
	
	assert ( equals(s3, firstObject(addRemove)) != 0 );
	assert ( equals(s3, lastObject(addRemove)) != 0 );
	
	assert( indexOfObject(addRemove, s2) == 1 );
	
	removeAllObjects(addRemove);
	assert( getCollectionCount(addRemove) == 0 );
	
	addObject(addRemove, s1);
	addObject(addRemove, s2);
	addObject(addRemove, s3);
	
	removeObjectAtIndex(addRemove, 1);
	
	assert( indexOfObject(addRemove, s2) == ANotFound );
	removeObject(addRemove, s3);
	assert( indexOfObject(addRemove, s3) == ANotFound );
	
	assert( indexOfObject(addRemove, s1) == 0 );
	assert( arrayContainsObject(addRemove, s1) );
	assert( ! arrayContainsObject(addRemove, s2) );
	
	removeAllObjects(addRemove);
	
	insertObjectAtIndex(addRemove, s1, 0);
	assert( arrayContainsObject(addRemove, s1) );
	assert( firstObject(addRemove) == s1);
	assert( lastObject(addRemove) == s1);
	
	insertObjectAtIndex(addRemove, s2, 1);
	assert( lastObject(addRemove) == s2);
	assert( getCollectionCount(addRemove) == 2);
	
	insertObjectAtIndex(addRemove, s3, 1);
	assert( indexOfObject(addRemove, s3) == 1);
	
	removeLastObject(addRemove);
	assert( ! arrayContainsObject(addRemove, s2) );
	
	release(s1);
	release(s2);
	release(s3);
	
	release(addRemove);
	
	{ /* Testing removeObjectsInRange */
		MutableArrayRef array = new(MutableArray, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef string = newStringWithFormat(String, "String %d", i, NULL);
			addObject(array, string);
			release(string);
		}
		
		SRange range = SMakeRange(5, 4);
		removeObjectsInRange(array, range);
		
		assert( getCollectionCount(array) == 6 );
		assert( strcmp("String 9", getStringText(getObjectAtIndex(array, 5))) == 0 );
		
		release(array);
	}
	
	{/* Testing replaceObjectAtIndex */
		MutableArrayRef array = new(MutableArray, NULL);
		
		for (int i=0; i<10; i++) {
			StringRef string = newStringWithFormat(String, "String %d", i, NULL);
			addObject(array, string);
			release(string);
		}
		
		StringRef string = new(String, "String 42", NULL);
		replaceObjectAtIndexWithObject(array, 5, string);
		release(string);
		
		assert( getCollectionCount(array) == 10 );
		assert( strcmp("String 42", getStringText(getObjectAtIndex(array, 5))) == 0 );
		
		release(array);
	}
	
	
	
//	printf("\nTests failed: %d\n", tests_failed);
//	printf("Tests successed: %d\n", tests_run - tests_failed);
//	printf("Tests run: %d\n", tests_run);

	return EXIT_SUCCESS;
}
