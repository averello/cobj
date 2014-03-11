//
//  MutableArray.r
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableArray_r
#define CObjects_MutableArray_r

#include <Array.h>
#include <Array.r>
#include <sys/queue.h>

TAILQ_HEAD(StoreHead, _Item);

struct _Item {
	const void * item;
	TAILQ_ENTRY(_Item) items;
};


CO_BEGIN_CLASS_TYPE_DECL(MutableArray,Array)
CO_END_CLASS_TYPE_DECL


CO_BEGIN_CLASS_DECL(MutableArrayClass,ArrayClass)
	void (* insertObject) (void *const self, void * const object);
	void (* addObject) (void *const self, void * const object);

	void (* removeFirstObject) (void *const self);
	void ( *removeLastObject) (void *const self);
	void (* removeAllObjects) (void *const self);
	
	void (* insertObjectAtIndex) (void *const self, void *const object, UInteger index);
	
	void (* removeObject) (void *const self, void * const object);
	void (* removeObjectAtIndex) (void *const self, UInteger index);
	void ( *removeObjectsInRange )(void *const self, Range range);
	
	void ( *replaceObjectAtIndexWithObject) (void *const self, UInteger index, void *const other);
	
	ObjectRef ( * popObject ) (void *const self);
CO_END_CLASS_DECL

//static struct StoreHead * MutableArray_getStore(const void * const _self);

#endif

