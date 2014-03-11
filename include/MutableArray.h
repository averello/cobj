//
//  MutableArray.h
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableArray_h
#define CObjects_MutableArray_h

#include <Array.h>
#include <StringObject.h>

extern const void * MutableArray;
extern const void * MutableArrayClass;
typedef void * MutableArrayRef;

void initMutableArray () __attribute__ ((constructor));
void deallocMutableArray () __attribute__ ((destructor));

ArrayRef newArrayWithArray(const void * const array);
ArrayRef newArrayFromMutableArray(const void * const mutableArray);
MutableArrayRef newMutableArrayFromArray(const void * const array);

void addObject(void *const self, void * const object);
void insertObject(void *const self, void * const object);
void insertObjectAtIndex(void *const self, void *const object, UInteger index);

void removeObject(void *const self, void * const object);
void removeObjectAtIndex(void *const self, UInteger index);
void removeObjectsInRange(void *const self, Range range);

void removeFirstObject(void *const self);
void removeLastObject(void *const self);
void removeAllObjects(void *const self);

void replaceObjectAtIndexWithObject(void *const self, UInteger index, void *const other);

ObjectRef popObject(void *const self);


#endif
