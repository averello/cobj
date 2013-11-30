//
//  MutableArray.c
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
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <sys/queue.h>

#include <cobj.h>
#include <new.h>
#include <MutableArray.h>
#include <MutableArray.r>

static void * MutableArray_constructor (void * _self, va_list * app) {
	struct MutableArray *self = super_constructor(Array, _self, app);
	struct Array *super = (struct Array *)self;
	
	void *item = NULL;
	va_list ap;
	va_copy(ap, *app);
	unsigned long itemsCount = 0;
	while ( (item = va_arg(ap, void *)) )
		itemsCount++;
	va_end(ap);
	
	
	TAILQ_HEAD(StoreHead, _Item) *store;
	store = calloc(1, sizeof(struct StoreHead));
	assert(store != NULL);
	TAILQ_INIT(store);
	
	super->count = itemsCount;
	
	if (itemsCount<=0) {
		super->store = store;
		return self;
	}

	while ( (item = va_arg(*app, void *)) ) {
		retain(item);
		struct _Item *_item = calloc(1, sizeof(struct _Item));
		_item->item = item;
		TAILQ_INSERT_TAIL(store, _item, items);
	}

	super->store = store;
	return self;
}

static void * MutableArray_destructor (void * _self) {
	struct Array *self = super_destructor(Array, _self);
	struct StoreHead * store =getStore(self);

	removeAllObjects(self);
	free(store), self->store = NULL, self->count = 0;
	return self;
}

static void * MutableArrayClass_constructor (void * _self, va_list *app) {
	struct MutableArrayClass * self = super_constructor(MutableArrayClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) insertObject )
			* (voidf *) & self->insertObject = method;
		else if (selector == (voidf) addObject )
			* (voidf *) & self->addObject = method;

		else if (selector == (voidf) removeFirstObject )
			* (voidf *) & self->removeFirstObject = method;
		else if (selector == (voidf) removeLastObject )
			* (voidf *) & self->removeLastObject = method;
		else if (selector == (voidf) removeAllObjects )
			* (voidf *) & self->removeAllObjects = method;

		else if (selector == (voidf) removeObject )
			* (voidf *) & self->removeObject = method;
		
		else if (selector == (voidf) insertObjectAtIndex )
			* (voidf *) & self->insertObjectAtIndex = method;
		else if (selector == (voidf) removeObjectAtIndex )
			* (voidf *) & self->removeObjectAtIndex = method;
		else if (selector == (voidf) removeObjectsInRange )
			* (voidf *) & self->removeObjectsInRange = method;
		else if (selector == (voidf) replaceObjectAtIndexWithObject )
			* (voidf *) & self->replaceObjectAtIndexWithObject = method;
		
		else if (selector == (voidf) popObject )
			* (voidf *) & self->popObject = method;
	}
	va_end(ap);
	return self;
}

static void *MutableArray_copy (const void *const _self) {
	const struct Array *arraySelf = _self;
	const struct MutableArray *self = _self;
	if ( arraySelf->count ==0 || getStore(self) == NULL)
		return new(MutableArray, NULL);
	
	struct Array * copyArray = new(MutableArray, NULL);
	ObjectRef item = NULL;
	unsigned long count = arraySelf->count;
	for (unsigned long i=0;  i<count && (item = getObjectAtIndex(self, i)) ; i++)
		addObject(copyArray, item);
	return (void *)copyArray;
}

static int MutableArray_equals (const void *const _self, const void *const other) {
	const struct Array *const self = _self;
	const struct Classs *const _super = superclass(superclass(classOf(_self)));
	
	int result = _super->equals(_self, other);
	if ( ! result) {
		result = (self->count == getArrayCount(other));
		if ( result ) {
			unsigned long size = self->count;
			for (unsigned long i=0; i<size && (result != 0); i++) {
				ObjectRef item1 = getObjectAtIndex(self, i);
				ObjectRef item2 = getObjectAtIndex(other, i);
				result = equals(item1, item2);
			}
		}
	}
	return result;
}

static void MutableArray_addObject (void *const self, const void *const object) {
	struct Array *super = self;
	if ( self != object )
		retain((void *)object);
	struct _Item *_item = calloc(1, sizeof(struct _Item));
	_item->item = object;
	struct StoreHead *store = getStore(self);
	TAILQ_INSERT_TAIL(store, _item, items);	
	super->count++;
}

static void MutableArray_insertObject (void *const self, const void *const object) {
	struct Array *const super = self;
	if ( self != object )
		retain((void *)object);
	struct _Item *const _item = calloc(1, sizeof(struct _Item));
	_item->item = object;
	struct StoreHead * store = getStore(self);
	TAILQ_INSERT_HEAD(store, _item, items);
	super->count++;
}

static ObjectRef MutableArray_getObjectAtIndex(const void * const _self, unsigned long index) {
	const struct Array *self = _self;
	struct StoreHead * store = getStore(self);
	assert(0 <= index && ( self->count != 0 ) ? ((self->count - index) > 0) : 1 );
	
	if ( store == NULL || self->count <= 0 )
		return NULL;
	unsigned long result = 0;
	struct _Item *it = NULL;
	for (it = store->tqh_first; it != NULL && result != index ; it = it->items.tqe_next, result++);
	return (void *)it->item;
}

static unsigned long MutableArray_indexOfObject(const void * const _self, const void * const object) {
	const struct Array *self = _self;
	struct StoreHead * store = getStore(self);
	
	if ( store == NULL || self->count <= 0 )
		return ANotFound;
	
	unsigned long result = 0;
	struct _Item *it = NULL;
//	for (it = store->tqh_first; it != NULL && it->item != object ; it = it->items.tqe_next, result++);
	for (it = store->tqh_first; it != NULL && ! equals(it->item, object) ; it = it->items.tqe_next, result++);
	if (result>=self->count)
		result = ANotFound;
	return result;
}

static int MutableArray_arrayContainsObject(const void * const self, const void * const object) {
	unsigned long result = indexOfObject(self, object);
	return (result != ANotFound);
}

static void MutableArray_removeAllObjects(void * const _self) {
	struct Array *const self = _self;
	unsigned long count = self->count;
	for (unsigned long i=0; i<count; i++)
		removeFirstObject(self);
}

static void * MutableArray_lastObject(const void * const _self) {
	const struct Array *const self = _self;
	if (self->count == 0 || self->store == NULL)
		return NULL;
	const struct StoreHead *const store = getStore(self);
	const struct _Item *const item =  (*(((struct StoreHead *)((store)->tqh_last))->tqh_last)); //TAILQ_LAST(store, StoreHead);
	return (void *) item->item;
}

static void * MutableArray_firstObject(const void * const _self) {
	const struct Array *const self = _self;
	if (self->count == 0 || self->store == NULL)
		return NULL;
	const struct StoreHead *const store = getStore(self);
	const struct _Item *const item = (store->tqh_first);
	return (void *)item->item;
}

static void MutableArray_removeLastObject(void * const _self) {
	struct Array *self = _self;
	if (self->count == 0 || self->store == NULL)
		return;
	removeObjectAtIndex(self, self->count-1);
}

static void MutableArray_removeFirstObject(void * const _self) {
	struct Array *self = _self;
	if (self->count == 0 || self->store == NULL)
		return;
	removeObjectAtIndex(self, 0);
}

static void MutableArray_insertObjectAtIndex(void *const _self, void *const object, unsigned long index) {
	struct Array *const self = _self;
	const struct StoreHead *const store = getStore(self);
	assert(index <= self->count);
	
	if (index==0) {
		insertObject(self, object);
		return;
	}
	else if (index==self->count) {
		addObject(self, object);
		return;
	}
	
	
	struct _Item *it = NULL;
	unsigned long result = 0;
	for (it = store->tqh_first; it != NULL && (index != result); it = it->items.tqe_next, result++);
	
	
	struct _Item * item = calloc(1, sizeof(struct _Item));
	item->item = object;
	if ( self != object )
		retain((void *)object);
	self->count++;
	/* TAILQ_INSERT_BEFORE(it, item, items); */
#ifdef DQUEUE_MACRO_DEBUG
	do { (item)->items.tqe_prev = (it)->items.tqe_prev; (((item))->items.tqe_next) = (it); *(it)->items.tqe_prev = (item); (it)->items.tqe_prev = &(((item))->items.tqe_next); do { (&(item)->items)->trace.prevline = (&(item)->items)->trace.lastline; (&(item)->items)->trace.prevfile = (&(item)->items)->trace.lastfile; (&(item)->items)->trace.lastline = __LINE__; (&(item)->items)->trace.lastfile = __FILE__; } while (0); do { (&it->items)->trace.prevline = (&it->items)->trace.lastline; (&it->items)->trace.prevfile = (&it->items)->trace.lastfile; (&it->items)->trace.lastline = __LINE__; (&it->items)->trace.lastfile = __FILE__; } while (0); } while (0);
#else
	 do {
		 (item)->items.tqe_prev = (it)->items.tqe_prev;
		 (((item))->items.tqe_next) = (it);
		 *(it)->items.tqe_prev = (item);
		 (it)->items.tqe_prev = &(((item))->items.tqe_next);
	 } while (0);
#endif
}

static struct _Item * MutableArray_ItemForIndex(const void *const _self, unsigned long index) {
	const struct Array *const self = _self;
	const struct StoreHead *const store = getStore(self);
	if (index >= self->count || self->count == 0)
		return NULL;
	
	struct _Item *it = NULL;
	unsigned long result = 0;
	for (it = store->tqh_first; it != NULL && (index != result); it = it->items.tqe_next, result++);
	return it;
}

static void MutableArray_removeObject(void *const _self, const void * const object) {
	struct Array *const self = _self;
	unsigned long index = indexOfObject(self, object);
	if (index != ANotFound)
		removeObjectAtIndex(self, index);
}

static void MutableArray_removeObjectAtIndex(void *const _self, unsigned long index) {
	struct Array *const self = _self;
	struct StoreHead *const store = getStore(self);
	if ( self->count == 0 || index >= self->count )
		return;
	
	struct _Item *item = MutableArray_ItemForIndex(self, index);
	if (item!=NULL) {		
		self->count --;
		release((void *)item->item);
		TAILQ_REMOVE(store, item, items);
		free(item);
	}
}

static void MutableArray_removeObjectsInRange(void *const _self, SRange range) {
	struct Array *const self = _self;
	if ( range.location >= self->count || SMaxRange(range) >= self->count ) return;
	
	for (unsigned long i=range.location; i<SMaxRange(range); i++)
		removeObjectAtIndex(self, range.location);
}

static void MutableArray_replaceObjectAtIndexWithObject(void *const _self, unsigned long index, void *const other) {
	struct Array *const self = _self;
	if ( index > self->count ) return;
	
	removeObjectAtIndex(_self, index);
	insertObjectAtIndex(_self, other, index);
}

static ObjectRef MutableArray_popObject(void *const _self) {
	struct Array *const self = _self;
	if ( self->count <= 0 ) return (ObjectRef)NULL;
	ObjectRef o = getObjectAtIndex(self, 0);
	if ( o != self ) retain(o);
	removeObjectAtIndex(self, 0);
	return o;
}

const void * MutableArray  = NULL;
const void * MutableArrayClass = NULL;

void initMutableArray () {
	initArray();
	if ( ! MutableArrayClass ) {
			MutableArrayClass = new(ArrayClass, "MutableArrayClass", ArrayClass, sizeof(struct MutableArrayClass), constructor, MutableArrayClass_constructor, NULL);
	}
	if ( !MutableArray ) {
		MutableArray = new(MutableArrayClass, "MutableArray", Array, sizeof(struct MutableArray),
						   constructor, MutableArray_constructor,
						   destructor, MutableArray_destructor,
						   copy, MutableArray_copy,
						   equals, MutableArray_equals,
						   
						   /* Overrides */
						   getObjectAtIndex, MutableArray_getObjectAtIndex,
						   firstObject, MutableArray_firstObject,
						   lastObject, MutableArray_lastObject,
						   indexOfObject, MutableArray_indexOfObject,
						   arrayContainsObject, MutableArray_arrayContainsObject,
						   
						   /* new */
						   addObject, MutableArray_addObject,
						   insertObject, MutableArray_insertObject,
						   insertObjectAtIndex, MutableArray_insertObjectAtIndex,
						   
						   removeObject, MutableArray_removeObject,
						   removeObjectAtIndex, MutableArray_removeObjectAtIndex,
						   removeObjectsInRange, MutableArray_removeObjectsInRange,
						   
						   removeFirstObject, MutableArray_removeFirstObject,
						   removeLastObject, MutableArray_removeLastObject,
						   removeAllObjects, MutableArray_removeAllObjects,
						   
						   replaceObjectAtIndexWithObject, MutableArray_replaceObjectAtIndexWithObject,
						   popObject, MutableArray_popObject,
						   NULL);
	}

}

void deallocMutableArray () {
	free((void *)MutableArray);
	MutableArray = NULL;
	free((void *)MutableArrayClass);
	MutableArrayClass = NULL;
	deallocArray();
}

//#pragma mark API
/* */

void addObject(void *const self, void * const object) {
	assert(self != NULL);
	assert(object != NULL);
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->addObject != NULL);
	class->addObject(self, object);
}

void insertObject(void *const self, void * const object) {
	assert(self != NULL);
	assert(object != NULL);
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->insertObject != NULL);
	class->insertObject(self, object);
}

void removeAllObjects(void *const self) {
	assert(self != NULL);
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeAllObjects != NULL);
	class->removeAllObjects(self);
}

void removeLastObject(void *const self) {
	assert( self != NULL );
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeLastObject != NULL);
	class->removeLastObject(self);
}

void removeFirstObject(void *const self) {
	assert( self != NULL );
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeFirstObject != NULL);
	class->removeFirstObject(self);
}

void insertObjectAtIndex(void *const self, void *const object, unsigned long index) {
	assert( self != NULL );
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->insertObjectAtIndex != NULL);
	class->insertObjectAtIndex(self, object, index);
}

void removeObject(void *const self, void * const object) {
	assert( self != NULL );
	assert( object != NULL );
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeObject != NULL);
	class->removeObject(self, object);
}

void removeObjectAtIndex(void *const self, unsigned long index) {
	assert( self != NULL );
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeObjectAtIndex != NULL);
	class->removeObjectAtIndex(self, index);
}

ArrayRef newArrayFromMutableArray(const void * const mutableArray) {
	assert( mutableArray != NULL );
	const unsigned long count = getArrayCount(mutableArray);
	struct _Bucket *newBuckets = calloc(count, sizeof(struct _Bucket));
	
	for (unsigned long i=0; i<count; i++) {
		ObjectRef item = getObjectAtIndex(mutableArray, i);
		retain(item);
		newBuckets[i].item = item;
	}

	struct Array *const newArray = new(Array, NULL);
	newArray->store = newBuckets;
	newArray->count = count;
	return newArray;
}

MutableArrayRef newMutableArrayFromArray(const void * const array) {
	assert( array != NULL );
	MutableArrayRef mutableArray = new(MutableArray, NULL);
	const unsigned long count = getArrayCount(array);
	for (unsigned long i=0; i<count; i++)
		addObject(mutableArray, getObjectAtIndex(array, i));
	return mutableArray;
}

/*
static void __initializeBuckets(const struct _Bucket * oldStore, struct _Bucket * newStore, unsigned long count) {
	for (unsigned long i=0; i<count; i++) {
		ObjectRef item = (ObjectRef)oldStore[i].item;
		retain(item);
		newStore[i].item = item;
	}
}
 */

ArrayRef newArrayWithArray(const void * const array) {
	assert( array != NULL );
	return copy(array);
	/*
	const struct _Bucket *const buckets = getStore(array);
	const unsigned long count = getArrayCount(array);
	struct _Bucket *newBuckets = calloc(count, sizeof(struct _Bucket));
	
	__initializeBuckets(buckets, newBuckets, count);
	
	struct Array *const newArray = new(Array, NULL);
	newArray->count = count;
	newArray->store = newBuckets;
	return newArray;
	 */
}

void removeObjectsInRange(void *const self, SRange range) {
	assert( self != NULL );
	if ( self == NULL ) return;
	
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeObjectsInRange != NULL);
	if ( class == NULL || class->removeObjectsInRange == NULL ) return;
	
	class->removeObjectsInRange(self, range);
}

void replaceObjectAtIndexWithObject(void *const self, unsigned long index, void *const other) {
	assert( self != NULL );
	if ( self == NULL ) return;
	
	assert( other != NULL );
	if ( other== NULL ) return;
	
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->removeObjectsInRange != NULL);
	if ( class == NULL || class->replaceObjectAtIndexWithObject == NULL ) return;
	
	class->replaceObjectAtIndexWithObject(self, index, other);
}

ObjectRef popObject(void *const self) {
	assert( self != NULL );
	if ( self == NULL ) return errno = EINVAL, (ObjectRef)NULL;
	
	const struct MutableArrayClass *class = classOf(self);
	assert(class != NULL && class->popObject != NULL);
	if ( class == NULL || class->popObject == NULL ) return errno = ENOSYS, (ObjectRef)NULL;
	
	return class->popObject(self);
}

