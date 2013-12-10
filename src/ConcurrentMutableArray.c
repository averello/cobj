/*!
 *  @file ConcurrentMutableArray.c
 *  @brief CObjects
 *
 *  Created by @author George Boumis
 *  @date 27/4/13.
 *  @copyright   Copyright (c) 2013 George Boumis. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <sys/queue.h>

#include <cobj.h>
#include <new.h>
#include <ConcurrentMutableArray.h>
#include <ConcurrentMutableArray.r>


static void * ConcurrentMutableArray_constructor (void * _self, va_list * app) {
	struct ConcurrentMutableArray *self = super_constructor(ConcurrentMutableArray, _self, app);
	pthread_mutex_init(&(self->protector), NULL);
	pthread_cond_init(&(self->synchronization), NULL);
	return self;
}

static void * ConcurrentMutableArray_destructor (void * _self) {
	struct ConcurrentMutableArray_destructor *self = super_destructor(ConcurrentMutableArray, _self);
	return self;
}

static void * ConcurrentMutableArrayClass_constructor (void * _self, va_list *app) {
	struct MutableArrayClass * self = super_constructor(ConcurrentMutableArrayClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
//		voidf method = va_arg(ap, voidf);
//		if (selector == (voidf) insertObject )
//			* (voidf *) & self->insertObject = method;
//		else if (selector == (voidf) addObject )
//			* (voidf *) & self->addObject = method;

	}
	va_end(ap);
	return self;
}

static unsigned long ConcurrentMutableArray_getCollectionCount(const void * const _self) {
	struct ConcurrentMutableArray *self = (struct ConcurrentMutableArray *)_self;
	const struct ArrayClass *const _superSuper = superclass(superclass(classOf(_self)));
	
	pthread_mutex_lock(&(self->protector));
	unsigned long count = _superSuper->getCollectionCount(self);
	pthread_mutex_unlock(&(self->protector));
	return count;
}

static ObjectRef ConcurrentMutableArray_getObjectAtIndex(const void * const _self, unsigned long index) {
	struct ConcurrentMutableArray *self = (struct ConcurrentMutableArray *)_self;
	const struct ArrayClass *const _superSuper = superclass(superclass(classOf(_self)));
	const struct ArrayClass *const _super = superclass(classOf(_self));

	
	pthread_mutex_lock(&(self->protector));
	unsigned long count = _superSuper->getCollectionCount(self);
	if ( index > count && index != 0 ) return pthread_mutex_unlock(&(self->protector)), errno = EINVAL, NULL;
	if ( count == 0 )
		pthread_cond_wait(&(self->synchronization),&(self->protector));
	ObjectRef o = _super->getObjectAtIndex(self, index);
	pthread_mutex_unlock(&(self->protector));
	return o;
}

static void ConcurrentMutableArray_addObject(void *const _self, void * const object) {
	struct ConcurrentMutableArray *self = _self;
	const struct ArrayClass *const _superSuper = superclass(superclass(classOf(_self)));
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	
	
	pthread_mutex_lock(&(self->protector));
	unsigned long count = _superSuper->getCollectionCount(self);
	_super->addObject(self, object);
	if ( count == 0 )
		pthread_cond_signal(&(self->synchronization));
	pthread_mutex_unlock(&(self->protector));
}

static void ConcurrentMutableArray_insertObject(void *const _self, void * const object) {
	struct ConcurrentMutableArray *self = _self;
	const struct ArrayClass *const _superSuper = superclass(superclass(classOf(_self)));
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	
	pthread_mutex_lock(&(self->protector));
	unsigned long count = _superSuper->getCollectionCount(self);
	_super->insertObject(self, object);
	if ( count == 0 )
		pthread_cond_signal(&(self->synchronization));
	pthread_mutex_unlock(&(self->protector));
}

static void ConcurrentMutableArray_insertObjectAtIndex(void *const _self, void *const object, unsigned long index) {
	struct ConcurrentMutableArray *self = _self;
	const struct ArrayClass *const _superSuper = superclass(superclass(classOf(_self)));
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	
	pthread_mutex_lock(&(self->protector));
	unsigned long count = _superSuper->getCollectionCount(self);
	_super->insertObjectAtIndex(_self, object, index);
	if ( count == 0 && index ) { errno = EINVAL; pthread_mutex_unlock(&(self->protector)); return; }
	if ( count == 0 )
		pthread_cond_signal(&(self->synchronization));
	pthread_mutex_unlock(&(self->protector));
}

static void *ConcurrentMutableArray_copy(const void *const _self) {
	struct ConcurrentMutableArray *self = (struct ConcurrentMutableArray *)_self;
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	const struct ArrayClass *const _superSuper = (const struct ArrayClass *)_super;
	
	pthread_mutex_lock(&(self->protector));
	const struct Array *arraySelf = _self;
	if ( arraySelf->count ==0 || getStore(self) == NULL)
		return pthread_mutex_unlock(&(self->protector)), new(MutableArray, NULL);
	
	struct Array * copyArray = new(MutableArray, NULL);
	ObjectRef item = NULL;
	unsigned long count = arraySelf->count;
	for (unsigned long i=0;  i<count && (item = _superSuper->getObjectAtIndex(self, i)) ; i++)
		_super->addObject(copyArray, item);
	pthread_mutex_unlock(&(self->protector));
	return (void *)copyArray;
}

static void ConcurrentMutableArray_removeObjectAtIndex(void *const _self, unsigned long index) {
	struct ConcurrentMutableArray *self = _self;
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	
	pthread_mutex_lock(&(self->protector));
	_super->removeObjectAtIndex(self, index);
	pthread_mutex_unlock(&(self->protector));
}

static ObjectRef ConcurrentMutableArray_popObject(void *const _self) {
	struct Array *const arraySelf = _self;
	struct ConcurrentMutableArray *self = _self;
	const struct MutableArrayClass *const _super = superclass(classOf(_self));
	const struct ArrayClass *const _superSuper = (const struct ArrayClass *)_super;
	
	pthread_mutex_lock(&(self->protector));
	if ( arraySelf->count <= 0 ) return pthread_mutex_unlock(&(self->protector)), (ObjectRef)NULL;
	ObjectRef o =  _superSuper->getObjectAtIndex(self, 0);
	if ( o != self ) retain(o);
	_super->removeObjectAtIndex(self, 0);
	pthread_mutex_unlock(&(self->protector));
	return o;
}

const void * ConcurrentMutableArray  = NULL;
const void * ConcurrentMutableArrayClass = NULL;

void initConcurrentMutableArray () {
	initArray();
	initMutableArray();
	
	if ( ! ConcurrentMutableArrayClass )
		ConcurrentMutableArrayClass = new(MutableArrayClass, "ConcurrentMutableArrayClass", MutableArrayClass, sizeof(struct ConcurrentMutableArrayClass),
			constructor, ConcurrentMutableArrayClass_constructor, NULL);
	if ( ! ConcurrentMutableArray )
		ConcurrentMutableArray = new(ConcurrentMutableArrayClass, "ConcurrentMutableArray", MutableArray, sizeof(struct ConcurrentMutableArray),
									 constructor, ConcurrentMutableArray_constructor,
									 destructor, ConcurrentMutableArray_destructor,
									 
									 /* Overrides */
									 copy, ConcurrentMutableArray_copy,
									 
									 getCollectionCount, ConcurrentMutableArray_getCollectionCount,
									 getObjectAtIndex, ConcurrentMutableArray_getObjectAtIndex,
									 
									 addObject, ConcurrentMutableArray_addObject,
									 insertObject, ConcurrentMutableArray_insertObject,
									 insertObjectAtIndex, ConcurrentMutableArray_insertObjectAtIndex,
									 removeObjectAtIndex, ConcurrentMutableArray_removeObjectAtIndex,
									 popObject, ConcurrentMutableArray_popObject,
									 
									 /* new */
									 
									 NULL);
}

void deallocConcurrentMutableArray () {
//	free((void *)ConcurrentMutableArray), ConcurrentMutableArray = NULL;
//	free((void *)ConcurrentMutableArrayClass), ConcurrentMutableArrayClass = NULL;
	release((void *)ConcurrentMutableArray), ConcurrentMutableArray = NULL;
	release((void *)ConcurrentMutableArrayClass), ConcurrentMutableArrayClass = NULL;
	deallocMutableArray();
	deallocArray();
}

/* */

int arrayContainsObject(const void * const self, const void * const object);
unsigned long indexOfObject(const void * const self, const void * const object);

void * lastObject(const void * const self);
void * firstObject(const void * const self);

void removeObject(void *const self, void * const object);
void removeObjectsInRange(void *const self, SRange range);

void removeFirstObject(void *const self);
void removeLastObject(void *const self);
void removeAllObjects(void *const self);

void replaceObjectAtIndexWithObject(void *const self, unsigned long index, void *const other);




