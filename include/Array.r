//
//  Array.r
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Array_r
#define CObjects_Array_r

#include <Object.h>
#include <Object.r>
#include <Collection.h>
#include <Collection.r>
#include <stdint.h>

struct _Bucket {
	const void *item;
};

struct Array {
	const struct Collection isa;
	uint64_t count;
	const void *store;
};

struct ArrayClass {
	const struct CollectionClass isa;
	unsigned long (* getCollectionCount)(const void * const _self);
	ObjectRef (* getObjectAtIndex)(const void * const _self, unsigned long index);
	
	int ( *arrayContainsObject) (const void * const self, const void * const object);
	unsigned long (* indexOfObject) (const void * const self, const void * const object);
//	ArrayRef (* newArrayWithItems) (const void * const _class, va_list *ap);
	void * ( * getStore) (const void * const self);
	
	void * (* lastObject) (const void * const self);
	void * (* firstObject) (const void * const self);
};

void *getStore(const void * const self);

#endif
