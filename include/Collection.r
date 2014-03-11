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

#include <stdint.h>

struct Collection {
	const struct Object isa;
};

struct CollectionClass {
	const struct Classs isa;
	uint64_t (*getCollectionCount)(const void *const self);
	void * (*lastObject)(const void * const self);
	void * (*firstObject)(const void * const self);
	int (*containsObject)(const void * const self, const void * const object);
	uint64_t (*enumerateWithState)(const void *const collection, FastEnumerationState *const state, void *iobuffer[], uint64_t length);
};

#endif
