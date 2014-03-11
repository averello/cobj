//
//  Array.c
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>


#include <coassert.h>
#include <codefinitions.h>

#include <new.h>
#include <Array.h>
#include <Array.r>
#include <StringObject.h>


extern int errno;

static bool Array_equals (const void *const _self, const void *const _other) CO_VISIBILITY_INTERNAL;

static void * Array_constructor (void * _self, va_list * app) {
	struct Array *self = super_constructor(Array, _self, app);
	
	void *item = NULL;
	va_list ap;
	va_copy(ap, *app);
	UInteger itemsCount = 0;
	while ( (item = va_arg(ap, void *)) )
		itemsCount++;
	va_end(ap);
	
	if (itemsCount<=0)
		return self;
		
	struct _Bucket *buckets = calloc(itemsCount, sizeof(struct _Bucket));
	assert(buckets != NULL);
	if (buckets == NULL) return free(self), NULL;
	
	self->count = itemsCount;
	
	UInteger i=0;
	while ( (item = va_arg(*app, void *)) ) {
		retain(item);
		buckets[i++].item = item;
	}
	
	self->store = buckets;
	return self;
}

static void * Array_destructor (void * _self) {
	struct Array *self = super_destructor(Array, _self);
	struct _Bucket *store = (struct _Bucket *)self->store;
	for (UInteger i=0; i<self->count; i++)
		release((void *)store[i].item);
	free(store), self->store = NULL, self->count = 0;
	return self;
}

static void * ArrayClass_constructor (void * _self, va_list *app) {
	struct ArrayClass * self = super_constructor(ArrayClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getObjectAtIndex)
			* (voidf *) & self->getObjectAtIndex = method;
		else if (selector == (voidf) indexOfObject )
			* (voidf *) & self->indexOfObject = method;
		else if (selector == (voidf) getStore )
			* (voidf *) & self->getStore = method;
	}
	va_end(ap);
	return self;
}

static void * Array_copy (const void *const _self) {
	const struct Array *self = _self;
//	if (getCollectionCount(self) ==0 || self->store == NULL)
	if (getCollectionCount(self) ==0 || self->store == NULL)
		return new(Array, NULL);
	
	UInteger size = self->count * sizeof(struct _Bucket);
	struct _Bucket *buckets = malloc(size);
	assert(buckets != NULL);
	if ( buckets == NULL ) return errno = ENOMEM, NULL;
	
	memcpy(buckets, self->store, size);
	for (UInteger i=0; i<getCollectionCount(self); i++)
		retain((void *)buckets[i].item);
	
	struct Array * copyArray = new(Array, NULL);
	copyArray->store = buckets;
	copyArray->count = getCollectionCount(self);
	return (void *)copyArray;
}

static struct _Bucket * Array_getStore(const void * const _self) {
	const struct Array *self = _self;
	return (struct _Bucket *)self->store;
}

static ObjectRef Array_getObjectAtIndex(const void * const _self, UInteger index) {
	const struct Array *self = _self;
	assert(0 <= index && index < self->count);
	if (self->count == 0 || self->store == NULL) return NULL;
	if ( index > self->count ) return errno = EINVAL, NULL;
	const struct _Bucket *store = getStore(self);
	if ( store != NULL )
		return (void *) store[index].item;
	return NULL;
}

static UInteger Array_getCollectionCount(const void * const _self) {
	const struct Array *self = _self;
	return self->count;
}

const void * Array = NULL;
const void * ArrayClass = NULL;

static bool Array_equals (const void *const _self, const void *const _other) {
	const struct Array *self = _self;
	const struct Array *other = _other;
	const struct Classs *const _super = (const struct Classs *const )super(_self);
	
	
	bool result = _super->equals(_self, _other);
	if ( ! result) {
		result = (getCollectionCount(self) == getCollectionCount(other));
		if ( result ) {
			UInteger size = getCollectionCount(self);
			for (UInteger i=0; i<size && (result != 0); i++) {
				ObjectRef item1 = getObjectAtIndex(self, i);
				ObjectRef item2 = getObjectAtIndex(other, i);
				result = equals(item1, item2);
			}
		}
	}
	return result;
}

static bool Array_arrayContainsObject(const void * const _self, const void * const _object) {
	const struct Array *self = _self;
	const struct Object *object = _object;
	
	bool result = NO;
	UInteger size = getCollectionCount(self);
	for (UInteger i=0; i<size && (result == 0); i++) {
		ObjectRef item = getObjectAtIndex(self, i);
		result = (equals(item, object));
	}
	return result;
}

static UInteger Array_indexOfObject(const void * const _self, const void * const _object) {
	const struct Array *self = _self;
	bool result = NO;
	UInteger index = NotFound;
	UInteger size = getCollectionCount(self);
	for (UInteger i=0; i<size && (result == 0); i++) {
		ObjectRef item = getObjectAtIndex(self, i);
		result = (equals(item, _object));
		if (result)
			index = i;
	}
	return index;
}

static StringRef Array_copyDescription(const void * const _self) {
	const struct Array *self = _self;
	const struct Class *const _super = (const struct Class *const )super(_self);
	
	StringRef supercopyDescription = copyDescription(_super);
	StringRef mycopyDescription = newStringWithFormat(String, "<%s {\n", getStringText(supercopyDescription), NULL);
	release(supercopyDescription);
	
	UInteger count = getCollectionCount(self);
	for (UInteger i=0; i<count; i++) {
		ObjectRef item = getObjectAtIndex(self, i);
		StringRef *string = copyDescription(item);
		StringRef *tmp = newStringWithFormat(String, "%s%s,\n", getStringText(mycopyDescription), getStringText(string), NULL);
		release(mycopyDescription);
		mycopyDescription = tmp;
		release(string);
	}

	StringRef *tmp = newStringWithFormat(String, "%s}", getStringText(mycopyDescription), NULL);
	release(mycopyDescription);
	mycopyDescription = tmp;
	return mycopyDescription;
}

static void * Array_lastObject(const void * const _self) {
	const struct Array *self = _self;
	if (self->count == 0 || self->store == NULL)
		return NULL;
	const struct _Bucket *store = getStore(self);
	if ( store != NULL )
		return (void *) store[self->count-1].item;
	return NULL;
}

static void * Array_firstObject(const void * const _self) {
	const struct Array *self = _self;
	if (self->count == 0 || self->store == NULL)
		return NULL;
	const struct _Bucket *store = getStore(self);
	if ( store != NULL )
		return (void *) store[0].item;
	return NULL;
}

//static Array_newArrayWithItems(const void * const _class, va_list *ap) {
//	const struct ArrayClass *const class = _class;
//}


//ArrayRef newArrayWithItems(const void * const _class, ...) {
//	assert(_class != NULL);
//	const struct ArrayClass *const class = _class;
//	assert(class != NULL && class->newArrayWithItems != NULL);
//	
//	va_list ap;
//	va_start(ap, _class);
//	ArrayRef array = class->newArrayWithItems(_class, &ap);
//	va_end(ap);
//	return array;
//};

static UInteger Array_enumerateWithState(const void *const _self, FastEnumerationState *const state, void *iobuffer[], UInteger length) {
	const struct Array *const self = _self;
	UInteger collectionCount = getCollectionCount(_self);
	if (state->state == 0) {
		state->mutationsPointer =(UInteger *)self;
		state->extra[0] = collectionCount;
		state->state = 1;
	}
	else
		if (collectionCount != state->extra[0])
			return state->mutationsPointer = NULL, 0;
	
	state->itemsPointer = iobuffer;
	UInteger count = 0;
	UInteger numberOfIter = MIN(collectionCount, length);
	for (UInteger i=(state->extra[1]), j=0; i<numberOfIter; i++, j++, count++)
		iobuffer[j] = getObjectAtIndex(self, i);
	if (count!=0)
		state->extra[1] = count;
	return count;
}

void initArray () {
	initCollection();
	
	if ( ! ArrayClass )
		ArrayClass = new(CollectionClass, "ArrayClass", CollectionClass, sizeof(struct ArrayClass), constructor, ArrayClass_constructor, NULL);
	if ( ! Array )
		Array = new(ArrayClass, "Array", Collection, sizeof(struct Array),
					constructor, Array_constructor,
					destructor, Array_destructor,
					
					/* Overrides */
					getCollectionCount, Array_getCollectionCount,
					firstObject, Array_firstObject,
					lastObject, Array_lastObject,
					containsObject, Array_arrayContainsObject,
					enumerateWithState, Array_enumerateWithState,
					
					/* new */
					copy, Array_copy,
					getObjectAtIndex,
					Array_getObjectAtIndex,
					equals, Array_equals,
					indexOfObject, Array_indexOfObject,
					copyDescription, Array_copyDescription,
					getStore, Array_getStore,
					NULL);
}

void deallocArray () {
	if (Array)
		release((void*)Array);
	if (ArrayClass)
		release((void*)ArrayClass);
//	free((void*)Array);
//	free((void*)ArrayClass);
	Array = NULL;
	ArrayClass = NULL;
	deallocCollection();
}

void *getStore(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct ArrayClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->getStore,ENOTSUP,NULL);
	return class->getStore(self);
}

ObjectRef getObjectAtIndex(const void * const self, UInteger index) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct ArrayClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->getObjectAtIndex,ENOTSUP,NULL);
	return class->getObjectAtIndex(self, index);
}

UInteger indexOfObject(const void * const self, const void * const object) {
	COAssertNoNullOrReturn(self,EINVAL,NotFound);
	COAssertNoNullOrReturn(object,EINVAL,NotFound);
	const struct ArrayClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NotFound);
	COAssertNoNullOrReturn(class->indexOfObject,ENOTSUP,NotFound);
	return class->indexOfObject(self, object);
}


