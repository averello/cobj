//
//  Dictionary.c
//  CObjects
//
//  Created by George Boumis on 14/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
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
	unsigned long count = 0;
	while ( (key = va_arg(ap, ObjectRef)) ) {
		va_arg(ap, ObjectRef);
		count++;
	}
	va_end(ap);
		
	if (count!=0) {
		MutableArrayRef keys = new(MutableArray, NULL);
		MutableArrayRef values = new(MutableArray, NULL);
		unsigned long *hashes = calloc(count, sizeof(unsigned long));
		
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

ObjectRef Dictionary_objectForKey(const void *const _self, void *const _key) {
	const struct Dictionary *const self = _self;
	unsigned long keyhash = hash(_key);
	unsigned long i;
	for (i=0; i<self->count && keyhash != self->hashes[i]; i++);
	if (i < self->count)
		return getObjectAtIndex(self->values, i);
	return NULL;
}

ObjectRef Dictionary_getKeysCopy(const void *const _self) {
	const struct Dictionary *const self = _self;
	return copy(self->keys);
}

ObjectRef Dictionary_getValuesCopy(const void *const _self) {
	const struct Dictionary *const self = _self;
	return copy(self->values);
}


ObjectRef Dictionary_copy(const void *const _self) {
	const struct Dictionary *const self = _self;
	struct Dictionary *newDictionary = new(Dictionary, NULL);
	if (self->count == 0)
		return newDictionary;
	
	newDictionary->keys = copy(self->keys);
	newDictionary->values = copy(self->values);
	newDictionary->count = self->count;
	unsigned long *hashes = calloc(self->count, sizeof(unsigned long));
	newDictionary->hashes = hashes;
	for (unsigned long i=0; i<self->count && (hashes[i] = self->hashes[i]); i++);
	return newDictionary;
}

unsigned long Dictionary_getCount(const void *const _self) {
	const struct Dictionary *const self = _self;
	return self->count;
}


const void *Dictionary = NULL;
const void *DictionaryClass = NULL;

void initDictionary() {
	initArray();
	initMutableArray();
	
	if ( ! DictionaryClass )
		DictionaryClass = new(Class, "DictionaryClass", Class, sizeof(struct DictionaryClass),
							  constructor, DictionaryClass_constructor,
							  NULL);
	if ( ! Dictionary )
		Dictionary = new(DictionaryClass, "Dictionary", Object, sizeof(struct Dictionary),
						 constructor, Dictionary_constructor,
						 destructor, Dictionary_destructor,
						 objectForKey, Dictionary_objectForKey,
//						 getCount, Dictionary_getCount,
						 getKeysCopy, Dictionary_getKeysCopy,
						 getValuesCopy, Dictionary_getValuesCopy,
						 NULL);
}

void deallocDictionary() {
	release((void *)Dictionary), Dictionary = NULL;
	release((void *)DictionaryClass), DictionaryClass = NULL;
//	free((void *)Dictionary), Dictionary = NULL;
//	free((void *)DictionaryClass), DictionaryClass = NULL;
	deallocArray();
	deallocMutableArray();
}


/* API */

ObjectRef objectForKey(const void *const self, void *const key) {
	assert( self != NULL );
	assert( key != NULL );
	const struct DictionaryClass *class = classOf(self);
	assert( class != NULL && class->objectForKey != NULL );
	return class->objectForKey(self, key);
}


ArrayRef getKeysCopy(const void *const self) {
	assert(self);
	const struct DictionaryClass *class = classOf(self);
	assert( class != NULL && class->getKeysCopy != NULL );
	return class->getKeysCopy(self);
}

ArrayRef getValuesCopy(const void *const self) {
	assert(self);
	const struct DictionaryClass *class = classOf(self);
	assert( class != NULL && class->getValuesCopy != NULL );
	return class->getValuesCopy(self);
}






