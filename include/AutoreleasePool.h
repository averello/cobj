//
//  AutoreleasePool.h
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_AutoreleasePool_h
#define CObjects_AutoreleasePool_h

/*! A @ref AutoreleasePool type. */
extern const void * AutoreleasePool;

/*! A @ref AutoreleasePoolClass type */
extern const void * AutoreleasePoolClass;
/*! An @ref AutoreleasePool reference. */
typedef void * AutoreleasePoolRef;

void addAutoreleaseObject(const void *self, const void *object);
void AutoreleasePoolAddObject(const void *object);

#endif
