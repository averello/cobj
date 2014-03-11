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

CO_DECLARE_CLASS(Collection)

UInteger getCollectionCount(const void *const self);
void * lastObject(const void * const self);
void * firstObject(const void * const self);
bool containsObject(const void * const self, const void * const object);
//UInteger enumerateWithState(ObjectRef collection, FastEnumerationState *state, ObjectRef iobuffer[], UInteger length);

#endif
