//
//  MutableDictionary.r
//  CObjects
//
//  Created by George Boumis on 15/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableDictionary_r
#define CObjects_MutableDictionary_r

#include <cobj.h>
#include <Object.r>
#include <Dictionary.r>

struct MutableDictionary {
	const struct Dictionary isa;
	VectorRef couples;
	void **level1; /* */
	
	/* if ( count > (size * loadFactor) ) => rehash() and double the size*/
	/* javadoc 7 HashMap */
	/* initialCapacity => size, number of entries => count
	 
	 An instance of HashMap has two parameters that affect its performance: initial capacity and load factor. The capacity is the number of buckets in the hash table, and the initial capacity is simply the capacity at the time the hash table is created. The load factor is a measure of how full the hash table is allowed to get before its capacity is automatically increased. When the number of entries in the hash table exceeds the product of the load factor and the current capacity, the hash table is rehashed (that is, internal data structures are rebuilt) so that the hash table has approximately twice the number of buckets.
	 
	 As a general rule, the default load factor (.75) offers a good tradeoff between time and space costs. Higher values decrease the space overhead but increase the lookup cost (reflected in most of the operations of the HashMap class, including get and put). The expected number of entries in the map and its load factor should be taken into account when setting its initial capacity, so as to minimize the number of rehash operations. If the initial capacity is greater than the maximum number of entries divided by the load factor, no rehash operations will ever occur.
	 */
//	unsigned long count;
	unsigned long size;
	float loadFactor;
};

struct MutableDictionaryClass {
	const struct DictionaryClass isa;
	void ( *setObjectForKey) (void *const self, void *const object, void *const key);
	void ( *setMutableDictionaryLoadFactor) (void *const self, float loadFactor);
	void ( *removeObjectForKey) (void *const self, void *const key);
};

#endif
