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

void initArray () __attribute__ ((constructor));
void deallocArray () __attribute__ ((destructor));

//ArrayRef newArrayWithItems(const void * const _class, ...);
ObjectRef getObjectAtIndex(const void * const self, UInteger index);

/* The index of the object or NotFound if there is no such object */
UInteger indexOfObject(const void * const self, const void * const object);

/* Sorting */
/* new array by adding items */

#endif
