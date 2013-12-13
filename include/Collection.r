//
//  Collection.r
//  CObjects
//
//  Created by George Boumis on 10/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Collection_r
#define CObjects_Collection_r

#include <Object.h>
#include <Object.r>
#include <foreach.h>

struct Collection {
	const struct Object isa;
};

struct CollectionClass {
	const struct Classs isa;
	unsigned long (*getCollectionCount)(const void *const self);
	void * (*lastObject)(const void * const self);
	void * (*firstObject)(const void * const self);
	int (*containsObject)(const void * const self, const void * const object);
	unsigned long (*enumerateWithState)(const void *const collection, FastEnumerationState *const state, void *iobuffer[], unsigned long length);
};

#endif
