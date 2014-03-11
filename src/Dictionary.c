//
//  Dictionary.c
//  CObjects
//
//  Created by George Boumis on 14/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <cobj.h>
#include <Dictionary.r>

static void * Dictionary_constructor (void * _self, va_list * app) {
	struct Dictionary *self = super_constructor(Dictionary, _self, app);
	
	
	va_list ap;
	va_copy(ap, *app);
	ObjectRef key = NULL, value = NULL;
	UInteger count = 0;
	while ( (key = va_arg(ap, ObjectRef)) ) {
		va_arg(ap, ObjectRef);
		count++;
	}
	va_end(ap);
		
	if (count!=0) {
		MutableArrayRef keys = new(MutableArray, NULL);
		MutableArrayRef values = new(MutableArray, NULL);
		UInteger *hashes = calloc(count, sizeof(UInteger));
		
		count = 0;
		while ( (key = va_arg(*app, ObjectRef)) ) {
			value = va_arg(*app, ObjectRef);
			addObject(keys, key);
			addObject(values, value);
			hashes[count] = hash(key);
			count++;
		}
		
		self->count = count;
		self->keys = newArrayFromMutableArray(keys);
		self->values = newArrayFromMutableArray(values);
		self->hashes = hashes;
		
		release(keys);
		release(values);
	}
	return self;
}

static void * Dictionary_destructor (void * _self) {
	struct Dictionary *self = super_destructor(Dictionary, _self);
	release(self->keys);
	release(self->values);
	free(self->hashes), self->hashes = NULL, self->count = 0, self->keys = NULL, self->values = NULL;;
	return self;
}

static void * DictionaryClass_constructor (void * _self, va_list *app) {
	struct DictionaryClass * self = super_constructor(DictionaryClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) objectForKey )
			* (voidf *) & self->objectForKey = method;
		
		else if (selector == (voidf) getKeysCopy )
			* (voidf *) & self->getKeysCopy = method;
		else if (selector == (voidf) getValuesCopy )
			* (voidf *) & self->getValuesCopy = method;
		
//		else if (selector == (voidf) getCount )
//			* (voidf *) & self->getCount = method;
	}
	va_end(ap);
	return self;
}

static ObjectRef Dictionary_objectForKey(const void *const _self, void *const _key) {
	const struct Dictionary *const self = _self;
	UInteger keyhash = hash(_key);
	UInteger i;
	for (i=0; i<self->count && keyhash != self->hashes[i]; i++);
	if (i < self->count)
		return getObjectAtIndex(self->values, i);
	return NULL;
}

static ObjectRef Dictionary_getKeysCopy(const void *const _self) {
	const struct Dictionary *const self = _self;
	return copy(self->keys);
}

static ObjectRef Dictionary_getValuesCopy(const void *const _self) {
	const struct Dictionary *const self = _self;
	return copy(self->values);
}


static ObjectRef Dictionary_copy(const void *const _self) {
	const struct Dictionary *const self = _self;
	struct Dictionary *newDictionary = new(Dictionary, NULL);
	if (self->count == 0)
		return newDictionary;
	
	newDictionary->keys = copy(self->keys);
	newDictionary->values = copy(self->values);
	newDictionary->count = self->count;
	UInteger *hashes = calloc(self->count, sizeof(UInteger));
	newDictionary->hashes = hashes;
	for (UInteger i=0; i<self->count && (hashes[i] = self->hashes[i]); i++);
	return newDictionary;
}

static UInteger Dictionary_getCount(const void *const _self) {
	const struct Dictionary *const self = _self;
	return self->count;
}

static UInteger Dictionary_enumerateWithState(const void *const _self, FastEnumerationState *const state, void *iobuffer[], UInteger length) {
	
	const struct Dictionary *const self = _self;
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
	ArrayRef allKeys = getKeysCopy(self);
	UInteger count = 0;
	UInteger numberOfIter = MIN(collectionCount, length);
	for (UInteger i=(state->extra[1]), j=0; i<numberOfIter; i++, j++, count++)
		iobuffer[j] = getObjectAtIndex(allKeys, i);
	if (count!=0)
		state->extra[1] = count;
	release(allKeys);
	return count;
}

const void *Dictionary = NULL;
const void *DictionaryClass = NULL;

void initDictionary() {
	initMutableArray();
	
	if ( ! DictionaryClass )
		DictionaryClass = new(CollectionClass, "DictionaryClass", CollectionClass, sizeof(struct DictionaryClass),
							  constructor, DictionaryClass_constructor,
							  NULL);
	if ( ! Dictionary )
		Dictionary = new(DictionaryClass, "Dictionary", Collection, sizeof(struct Dictionary),
						 constructor, Dictionary_constructor,
						 
						 /* Overrides */
						 copy, Dictionary_copy,
						 getCollectionCount, Dictionary_getCount,
						 enumerateWithState, Dictionary_enumerateWithState,
						 
						 /* new */
						 destructor, Dictionary_destructor,
						 objectForKey, Dictionary_objectForKey,
						 getKeysCopy, Dictionary_getKeysCopy,
						 getValuesCopy, Dictionary_getValuesCopy,
						 NULL);
}

void deallocDictionary() {
	if (Dictionary)
		release((void *)Dictionary), Dictionary = NULL;
	if (DictionaryClass)
		release((void *)DictionaryClass), DictionaryClass = NULL;
//	free((void *)Dictionary), Dictionary = NULL;
//	free((void *)DictionaryClass), DictionaryClass = NULL;
	deallocMutableArray();
}


/* API */

ObjectRef objectForKey(const void *const self, void *const key) {
	COAssertNoNullOrReturn(self, EINVAL, NULL);
	COAssertNoNullOrReturn(key, EINVAL, NULL);
	const struct DictionaryClass *class = classOf(self);
	COAssertNoNullOrReturn(class, EINVAL, NULL);
	COAssertNoNullOrReturn(class->objectForKey, ENOTSUP, NULL);
	return class->objectForKey(self, key);
}


ArrayRef getKeysCopy(const void *const self) {
	COAssertNoNullOrReturn(self, EINVAL, NULL);
	const struct DictionaryClass *class = classOf(self);
	COAssertNoNullOrReturn(class, EINVAL, NULL);
	COAssertNoNullOrReturn(class->getKeysCopy, ENOTSUP, NULL);
	return class->getKeysCopy(self);
}

ArrayRef getValuesCopy(const void *const self) {
	COAssertNoNullOrReturn(self, EINVAL, NULL);
	const struct DictionaryClass *class = classOf(self);
	COAssertNoNullOrReturn(class, EINVAL, NULL);
	COAssertNoNullOrReturn(class->getValuesCopy, ENOTSUP, NULL);
	return class->getValuesCopy(self);
}






