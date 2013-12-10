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

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

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

static unsigned long Collection_getCollectionCount(const void * const self) {
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
						 getCollectionCount, getCollectionCount,
						 firstObject, Collection_firstObject,
						 lastObject, Collection_lastObject,
						 containsObject, Collection_containsObject,
						 enumerateWithState, Collection_enumerateWithState,
					 NULL);
}

void deallocCollection() {
	release((void*)Collection);
	release((void*)CollectionClass);
	Collection = NULL;
	CollectionClass = NULL;
}

unsigned long getCollectionCount(const void * const self) {
	assert(self != NULL);
	if ( self == NULL ) return errno = EINVAL, 0;
	const struct CollectionClass *class = classOf(self);
	assert(class != NULL && class->getCollectionCount != NULL);
	if ( class == NULL || class->getCollectionCount == NULL ) return errno = ENOTSUP, 0;
	return class->getCollectionCount(self);
}

int containsObject(const void * const self, const void * const object) {
	assert(self != NULL);
	if ( self == NULL ) return errno = EINVAL, 0;
	assert(object != NULL);
	if ( object == NULL ) return errno = EINVAL, 0;
	const struct CollectionClass *class = classOf(self);
	assert(class != NULL && class->containsObject != NULL);
	if ( class == NULL || class->containsObject == NULL ) return errno = ENOTSUP, 0;
	return class->containsObject(self, object);
}

void * lastObject(const void * const self) {
	assert(self != NULL);
	const struct CollectionClass *class = classOf(self);
	assert(class != NULL && class->lastObject != NULL);
	if ( class == NULL || class->lastObject == NULL ) return errno = ENOTSUP, NULL;
	return class->lastObject(self);
}

void * firstObject(const void * const self) {
	assert(self != NULL);
	if ( self == NULL ) return errno = EINVAL, NULL;
	const struct CollectionClass *class = classOf(self);
	assert(class != NULL && class->firstObject != NULL);
	if ( class == NULL || class->firstObject == NULL ) return errno = ENOTSUP, NULL;
	return class->firstObject(self);
}

unsigned long enumerateWithState(ObjectRef collection, FastEnumerationState *state, ObjectRef iobuffer[], unsigned long length) {
	assert(collection != NULL);
	if (collection == NULL) return 0;
	assert(state != NULL);
	if (state == NULL) return 0;
	
	const struct CollectionClass *class = classOf(collection);
	assert(class != NULL && class->enumerateWithState != NULL);
	if ( class == NULL || class->enumerateWithState == NULL ) return errno = ENOTSUP, 0;
	return class->enumerateWithState(collection, state, iobuffer, length);
}


