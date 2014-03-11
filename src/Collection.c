//
//  Collection.c
//  CObjects
//
//  Created by George Boumis on 10/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

#include <coassert.h>

#include <Collection.h>
#include <Collection.r>
#include <new.h>
#include <Object.h>
#include <Object.r>

extern int errno;

static void * Collection_constructor (void * _self, va_list * app) {
	struct Collection *self = super_constructor(Collection, _self, app);
	return self;
}

static void * CollectionClass_constructor (void * _self, va_list *app) {
	struct CollectionClass * self = super_constructor(CollectionClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getCollectionCount)
			* (voidf *) & self->getCollectionCount = method;
		else if (selector == (voidf) lastObject )
			* (voidf *) & self->lastObject = method;
		else if (selector == (voidf) firstObject )
			* (voidf *) & self->firstObject = method;
		else if (selector == (voidf) containsObject )
			* (voidf *) & self->containsObject = method;
		else if (selector == (voidf) enumerateWithState )
			* (voidf *) & self->enumerateWithState = method;
	}
	va_end(ap);
	return self;
}

static void * Collection_destructor (void * _self) {
	struct Collection *self = super_destructor(Collection, _self);
	return self;
}

static uint64_t Collection_getCollectionCount(const void * const self) {
	return 0;
}

static int Collection_containsObject(const void * const self, const void * const object) {
	return 0;
}

static void * Collection_lastObject(const void * const self) {
	return NULL;
}

static void * Collection_firstObject(const void * const self) {
	return NULL;
}

static unsigned long Collection_enumerateWithState(ObjectRef collection, FastEnumerationState *state, ObjectRef iobuffer[], unsigned long length) {
	return 0;
}

const void * CollectionClass = NULL;
const void * Collection = NULL;

void initCollection() {
	if ( ! CollectionClass )
		CollectionClass = new(Class, "CollectionClass", Class, sizeof(struct CollectionClass),
							  constructor, CollectionClass_constructor, NULL);
	if ( ! Collection )
		Collection = new(CollectionClass, "Collection", Object, sizeof(struct Collection),
						 constructor, Collection_constructor,
						 destructor, Collection_destructor,
						 
						 /* new */
						 getCollectionCount, Collection_getCollectionCount,
						 firstObject, Collection_firstObject,
						 lastObject, Collection_lastObject,
						 containsObject, Collection_containsObject,
						 enumerateWithState, Collection_enumerateWithState,
					 NULL);
}

void deallocCollection() {
	if (Collection)
		release((void*)Collection);
	if (CollectionClass)
		release((void*)CollectionClass);
	Collection = NULL;
	CollectionClass = NULL;
}

uint64_t getCollectionCount(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	const struct CollectionClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->getCollectionCount,ENOTSUP,0);
	return class->getCollectionCount(self);
}

int containsObject(const void * const self, const void * const object) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	COAssertNoNullOrReturn(object,EINVAL,0);
	const struct CollectionClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->containsObject,ENOTSUP,0);
	return class->containsObject(self, object);
}

void * lastObject(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct CollectionClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->lastObject,ENOTSUP,NULL);
	return class->lastObject(self);
}

void * firstObject(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct CollectionClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->firstObject,ENOTSUP,NULL);
	return class->firstObject(self);
}

uint64_t enumerateWithState(const void *const collection, FastEnumerationState *const state, void *iobuffer[], uint64_t length) {
	COAssertNoNullOrReturn(collection,EINVAL,0);
	COAssertNoNullOrReturn(state,EINVAL,0);
	
	const struct CollectionClass *class = classOf(collection);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->enumerateWithState,ENOTSUP,0);
	return class->enumerateWithState(collection, state, iobuffer, length);
}


