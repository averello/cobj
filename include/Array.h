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
#include <Collection.h>
#include <limits.h>

extern const void * Array;
extern const void * ArrayClass;
typedef void * ArrayRef;

typedef enum {
	CONotFound = INT_MAX
} ASD;

void initArray () __attribute__ ((constructor));
void deallocArray () __attribute__ ((destructor));

//ArrayRef newArrayWithItems(const void * const _class, ...);
ObjectRef getObjectAtIndex(const void * const self, unsigned long index);

/* The index of the object or CONotFound if there is no such object */
unsigned long indexOfObject(const void * const self, const void * const object);

/* Sorting */
/* new array by adding items */

#endif
