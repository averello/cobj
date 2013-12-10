//
//  testMutableDictionary.c
//  CObjects
//
//  Created by averello on 3/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <cobj.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <time.h>
#include <unistd.h>


#ifndef __PROFILING__
#define PRINTF
#else
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#endif

int main () {
	StringRef k1, k2, k3;
	StringRef v1, v2, v3;
	k1 = new(String, "key 1", NULL);
	k2 = new(String, "key 2", NULL);
	k3 = new(String, "key 3", NULL);
	
	v1 = new(String, "value 1", NULL);
	v2 = new(String, "value 2", NULL);
	v3 = new(String, "value 3", NULL);
	
	/* Testing new */
	{
		MutableDictionaryRef dictionary = new(MutableDictionary, k1, v1, k2, v2, k3, v3, NULL);
		assert( dictionary != NULL );
		release(dictionary);
	}
	
	/* Testing objectForKey */
	{
		MutableDictionaryRef dictionary = new(MutableDictionary, k1, v1, k2, v2, k3, v3, NULL);
		
		ObjectRef value1 = objectForKey(dictionary, k1);
		assert( value1 != NULL );
		assert( value1 == v1 );
			
		StringRef kk1 = new(String, "key 1", NULL);
		ObjectRef value2 = objectForKey(dictionary, kk1);
		assert( value2 != NULL );
		assert( value2 == v1 );

		release(kk1);
		release(dictionary);
	}
	
	/* Testing setObjectForKey */
	{
		/* With prefilled Dictionary */
		{
			MutableDictionaryRef dictionary = new(MutableDictionary, k1, v1, k2, v2, k3, v3, NULL);
			
			setObjectForKey(dictionary, v3, k1);
			ObjectRef newValue = objectForKey(dictionary, k1);
			assert( newValue != NULL );
			assert( newValue == v3 );
			
			StringRef newKey = new(String, "this is a new key", NULL);
			newValue = new(String, "this is a new value adsjsdjhfbsdjf", NULL);
			setObjectForKey(dictionary, newValue, newKey);
			
			ObjectRef newValue2 = objectForKey(dictionary, newKey);
			assert( newValue2 != NULL );
			assert( newValue2 == newValue );
			
			release(newKey);
			release(newValue);
			release(dictionary);
		}
		/* With empty Dictionary */
		{
			MutableDictionaryRef dictionary = new(MutableDictionary, NULL);
			setObjectForKey(dictionary, v1, k1);
			ObjectRef newValue = objectForKey(dictionary, k1);
			assert( newValue != NULL );
			assert( newValue == v1 );
			
			ObjectRef noValue = objectForKey(dictionary, k3);
			assert( noValue == NULL );
			
			release(dictionary);
		}
	}
	
#ifdef __PROFILING__
	/* stress test */
	{
		#define PROFILE_SIZE 10000
		clock_t startTotal = clock();
		MutableDictionaryRef dictionary = new(MutableDictionary, NULL);
//		setMutableDictionaryLoadFactor(dictionary, 0.75);
		
		MutableArrayRef keys = new(MutableArray, NULL);
		MutableArrayRef values = new(MutableArray, NULL);
	
		srand(13);
		clock_t start = clock();
		for (unsigned long i=0; i<PROFILE_SIZE; i++) {
			int random = rand();
			StringRef key = newStringWithFormat(String, "key[%d]", random, NULL);
			StringRef value = newStringWithFormat(String, "value {[%d]}", random, NULL);
			addObject(keys, key);
			addObject(values, value);
			release(key);
			release(value);
		}
		PRINTF("String Allocation Time :%f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
		assert( getCollectionCount(keys) == getCollectionCount(values) && getCollectionCount(keys) == PROFILE_SIZE );
		
//		MutableDictionaryPrintfStatistics(dictionary);
		start = clock();
		for (unsigned long i=0; i<PROFILE_SIZE; i++) {
			StringRef key = getObjectAtIndex(keys, i);
			StringRef value = getObjectAtIndex(values, i);
			setObjectForKey(dictionary, value, key);
		}
		PRINTF("MutableDictionary setObjectForKey Time :%f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
		MutableDictionaryPrintfStatistics(dictionary);
		
		start = clock();
		for (unsigned long i=0; i<PROFILE_SIZE; i++) {
			StringRef key = getObjectAtIndex(keys, i);
			assert( objectForKey(dictionary, key) != NULL );
		}
		PRINTF("MutableDictionary objectForKey Time :%f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
		
		start = clock();
		for (unsigned long i=0; i<PROFILE_SIZE; i++) {
			StringRef key = getObjectAtIndex(keys, i);
			removeObjectForKey(dictionary, key);
			assert( objectForKey(dictionary, key) == NULL );
		}
		PRINTF("MutableDictionary removeObjectForKey Time :%f sec\n", (double)(clock()-start)/CLOCKS_PER_SEC);
		
		
		release(keys);
		release(values);
		release(dictionary);
		PRINTF("Total Time :%f sec\n", (double)(clock()-startTotal)/CLOCKS_PER_SEC);
	}
#endif
	
	/* Testing the change of a load Factor */
	{
//		MutableDictionaryRef dictionary = new(MutableDictionary, NULL);
	}
	
	/* Testing removeObjectForKey */
	{
		MutableDictionaryRef dictionary = new(MutableDictionary, NULL);
		setObjectForKey(dictionary, v1, k1);
		ObjectRef newValue = objectForKey(dictionary, k1);
		assert( newValue != NULL );
		assert( newValue == v1 );
		
		removeObjectForKey(dictionary, k1);
		
		ObjectRef noValue = objectForKey(dictionary, k1);
		assert( noValue == NULL );
		
		release(dictionary);
	}
	
	release(k1), release(k2), release(k3);
	release(v1), release(v2), release(v3);
	return 0;
}
