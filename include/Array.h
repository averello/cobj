//
//  Array.h
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Array_h
#define CObjects_Array_h

#include <Object.h>
#include <limits.h>

extern const void * Array;
extern const void * ArrayClass;
typedef void * ArrayRef;

enum {
	ANotFound = INT_MAX
};

void initArray () __attribute__ ((constructor));
void deallocArray () __attribute__ ((destructor));

//ArrayRef newArrayWithItems(const void * const _class, ...);
unsigned long getArrayCount(const void * const self);
ObjectRef getObjectAtIndex(const void * const self, unsigned long index);

/* bool return type */
int arrayContainsObject(const void * const self, const void * const object);

/* The index of the object or ANotFound if there is no such object */
unsigned long indexOfObject(const void * const self, const void * const object);

void * lastObject(const void * const self);
void * firstObject(const void * const self);

/* Sorting */
/* new array by adding items */

#endif
