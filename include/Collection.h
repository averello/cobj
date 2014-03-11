//
//  Collection.h
//  CObjects
//
//  Created by George Boumis on 10/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Collection_h
#define CObjects_Collection_h

#include <Object.h>
#include <stdint.h>

extern const void * Collection;
extern const void * CollectionClass;
typedef void * CollectionRef;

void initCollection() __attribute__ ((constructor));
void deallocCollection () __attribute__ ((destructor));

UInteger getCollectionCount(const void *const self);
void * lastObject(const void * const self);
void * firstObject(const void * const self);
bool containsObject(const void * const self, const void * const object);
//UInteger enumerateWithState(ObjectRef collection, FastEnumerationState *state, ObjectRef iobuffer[], UInteger length);

#endif
