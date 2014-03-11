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

CO_BEGIN_CLASS_DECL(Collection,Object)
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(CollectionClass,Classs)
	UInteger (*getCollectionCount)(const void *const self);
	void * (*lastObject)(const void * const self);
	void * (*firstObject)(const void * const self);
	int (*containsObject)(const void * const self, const void * const object);
	UInteger (*enumerateWithState)(const void *const collection, FastEnumerationState *const state, void *iobuffer[], UInteger length);
CO_END_CLASS_DECL

#endif
