/*!
 *  @file ConcurrentMutableArray.h
 *  @brief CObjects
 *
 *  Created by @author George Boumis 
 *  @date 27/4/13.
 *  @copyright   Copyright (c) 2013 George Boumis. All rights reserved.
 */

#ifndef CObjects_ConcurrentMutableArray_h
#define CObjects_ConcurrentMutableArray_h

#include <MutableArray.h>

extern const void * ConcurrentMutableArray;
extern const void * ConcurrentMutableArrayClass;
typedef void * ConcurrentMutableArrayRef;

void initConcurrentMutableArray () __attribute__ ((constructor));
void deallocConcurrentMutableArray () __attribute__ ((destructor));

/* Synchrinized methods working so far
 copy,
 
 getArrayCount,
 getObjectAtIndex,
 
 addObject,
 insertObject,
 insertObjectAtIndex,
 
 popObject,
 */

#endif
