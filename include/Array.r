//
//  Array.r
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Array_r
#define CObjects_Array_r

#include <Object.h>
#include <Object.r>
#include <Collection.h>
#include <Collection.r>
#include <codefinitions.h>

struct _Bucket {
	const void *item;
};

CO_BEGIN_CLASS_TYPE_DECL(Array,Collection)
	UInteger count;
	const void *store;
CO_END_CLASS_TYPE_DECL


CO_BEGIN_CLASS_DECL(ArrayClass,CollectionClass)
	ObjectRef (* getObjectAtIndex)(const void * const _self, UInteger index);
	UInteger (* indexOfObject) (const void * const self, const void * const object);
	void * ( * getStore) (const void * const self);
CO_END_CLASS_DECL

void *getStore(const void * const self);

#endif
