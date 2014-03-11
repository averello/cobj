//
//  Vector.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <cobj.h>
#include <Vector.r>
#include <Array.r>

const void * Vector = NULL;
const void * VectorClass = NULL;


static int __grow(struct Vector *const self, UInteger minCapacity) {
	// overflow-conscious code
	struct Array *arraySelf = (struct Array *)self;
	UInteger newCapacity = 0;
	UInteger oldCapacity = self->capacity;
	UInteger increment = ((self->capacityIncrement > 0) ? (UInteger)self->capacityIncrement : oldCapacity);
	if (UINT64_MAX - oldCapacity < increment ) {
		newCapacity = UINT64_MAX;
	}
	else {
		newCapacity = oldCapacity + increment;
	}

	const void *newStore = realloc((void *)arraySelf->store, newCapacity * sizeof(struct _Bucket) );
	if ( newStore == NULL ) return -1;
	self->capacity = newCapacity;
	arraySelf->store = newStore;
//	/* zero-out the remaining space */
//	memset( ((char *)arraySelf->store)+arraySelf->count, 0, (newCapacity - arraySelf->count) * sizeof(struct _Bucket) );
	return 0;
}

static int __shrink(struct Vector *const self, UInteger minCapacity) {
	struct Array *arraySelf = (struct Array *)self;
	UInteger newCapacity = minCapacity;
	const void *newStore = realloc((void *)arraySelf->store, newCapacity * sizeof(struct _Bucket) );
	if ( newStore == NULL ) return -1;
	self->capacity = newCapacity;
	arraySelf->store = newStore;
	return 0;
}

inline static int __ensureCapacity(struct Vector *const self, UInteger minCapacity) {
		if (
			(minCapacity - self->capacity > 0) )
			return __grow(self, minCapacity);
	return 0;
}


static void * Vector_constructor (void * _self, va_list * app) {
	struct Vector *self = super_constructor(Array, _self, app);
	struct Array *arraySelf = (struct Array *)self;
	
	int err;
	UInteger capacity = va_arg(*app, UInteger);
	UInteger capacityIncrement = va_arg(*app, UInteger);
	
	
	if ( capacity == 0)
		capacity = 10;
	self->capacity = capacity;
	self->capacityIncrement = capacityIncrement;
	
	arraySelf->store = calloc(capacity, sizeof(struct _Bucket));
	if ( arraySelf->store == NULL ) return err = errno, free(self), errno = err, NULL;
	
	return self;
}

static void * Vector_destructor (void * _self) {
	struct Vector *self = super_destructor(Array, _self);
	struct Array *arraySelf = (struct Array *)self;
	removeAllObjects(self);
	free((void *)arraySelf->store), arraySelf->store = NULL;
	return self;
}

static void * VectorClass_constructor (void * _self, va_list *app) {
	struct VectorClass * self = super_constructor(VectorClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getVectorCapacity )
			* (voidf *) & self->getVectorCapacity = method;
		else if (selector == (voidf) getVectorCapacityIncrement )
			* (voidf *) & self->getVectorCapacityIncrement = method;
		else if (selector == (voidf) setVectorSize )
			* (voidf *) & self->setVectorSize = method;
		else if (selector == (voidf) setVectorCapacityIncrement )
			* (voidf *) & self->setVectorCapacityIncrement = method;
	}
	va_end(ap);
	return self;
}

static void * Vector_copy (const void * const _self) {
	const struct Value *self = _self;
	const struct Array *arraySelf = _self;
	VectorRef newVectorRef = new(Vector, getVectorCapacity(self), getVectorCapacityIncrement(self), NULL);
	struct Array *newArrayVector = newVectorRef;
	newArrayVector->count = arraySelf->count;
	
	struct _Bucket *newVectorStore = getStore(newArrayVector);
	struct _Bucket *store = getStore(self);
	
	for (UInteger i=0; i<arraySelf->count; i++)
		retain((void *)store[i].item), newVectorStore[i].item = store[i].item;

	return newVectorRef;
}

static bool Vector_equals (const void * const _self, const void *const _other) {
	const struct Vector *self = _self;
	const struct Vector *other = _other;
	const struct Array *arraySelf = _self;
	const struct Array *arrayOther = _other;
	const struct Classs *const _super = (const struct Classs *const )super(_self);
	bool result = _super->equals(self, other);
	if ( ! result )
		result = (arraySelf->count == arrayOther->count) && ( memcmp(arraySelf->store, arrayOther->store, arraySelf->count * sizeof(void *)) );
	return result;
}

static UInteger Vector_getVectorCapacity(const void *const _self) {
	const struct Vector *self = _self;
	return self->capacity;
}

static UInteger Vector_getVectorCapacityIncrement(const void *const _self) {
	const struct Vector *self = _self;
	return self->capacityIncrement;
}

/* Overrides */
static void * Vector_getObjectAtIndex(const void * const _self, UInteger index) {
	const struct Vector *self = _self;
	if ( index >= getCollectionCount(self) ) return errno = EINVAL, NULL;
	struct _Bucket *store = getStore(self);
	return (void *)store[index].item;
}

static void * Vector_lastObject(const void * const _self) {
	const struct Array *self = _self;
	if (self->count == 0) return errno = EACCES, NULL;
	struct _Bucket *store = getStore(self);
	return (void *)store[self->count-1].item;
}

static void * Vector_firstObject(const void * const _self) {
	const struct Array *self = _self;
	if (self->count == 0) return errno = EACCES, NULL;
	struct _Bucket *store = getStore(self);
	return (void *)store[0].item;
}

static void Vector_addObject(void *const _self, void * const object) {
	struct Array *self = _self;
	if ( __ensureCapacity(_self, self->count+1) == -1 ) { errno = ENOMEM; return; }
	struct _Bucket *store = getStore(_self);
	store[self->count++].item = object;
	if ( self != object )
		retain(object);
}

static void Vector_insertObject(void *const self, void * const object) {
	insertObjectAtIndex(self, object, 0);
}
static void Vector_insertObjectAtIndex(void *const _self, void *const object, UInteger index) {
	struct Array *self = _self;
	errno = 0;
	
	if ( index > getCollectionCount(self) ) { errno = EINVAL; return; }
	if ( __ensureCapacity(_self, self->count+1) == -1 ) { errno = ENOMEM; return; }
	struct _Bucket *store = getStore(_self);
	memmove(store+index+1, store+index, (self->count - index) * sizeof(struct _Bucket));
	store[index].item = object;
	if ( self != object )
		retain(object);
	self->count++;
}

static void Vector_removeObjectAtIndex(void *const _self, UInteger index) {
	struct Array *self = _self;
	errno = 0;
	
	if ( index >= self->count ) { errno = EINVAL; return; }
	
	struct _Bucket *store = getStore(_self);
	release((void *)store[index].item);
	store[index].item = NULL;
	
	UInteger numMoved = self->count - index - 1;
	if ( numMoved > 0 ) {
		memmove(store + index, store + index + 1, numMoved * sizeof(struct _Bucket));
//		memset(store+index + numMoved, 0, numMoved * sizeof(struct _Bucket));
	}
	self->count--;
}

static UInteger Vector_indexOfObject(const void * const _self, const void * const object) {
	const struct Array *self = _self;
	int result = 0;
	UInteger index = NotFound;
	UInteger size = self->count;
	for (UInteger i=0; i<size && (result == 0); i++) {
		ObjectRef item = getObjectAtIndex(self, i);
		result = (equals(item, object));
		if (result)
			index = i;
	}
	return index;
}

static bool Vector_arrayContainsObject(const void * const self, const void * const object) {
	return (indexOfObject(self, object) != NotFound);
}

static void Vector_removeObject(void *const self, const void * const object) {
	errno = 0;
	UInteger index = indexOfObject(self, object);
	if ( index != NotFound )
		removeObjectAtIndex(self, index);
	else
		errno = EINVAL;
}


static void Vector_removeFirstObject(void *const self) {
	removeObjectAtIndex(self, 0);
}

static void Vector_removeLastObject(void *const self) {
	removeObjectAtIndex(self, getCollectionCount(self)-1);
}

static void Vector_removeAllObjects(void *const _self) {
	struct Array *self = _self;
	UInteger count = self->count;
	struct _Bucket *store = getStore(_self);
	for (UInteger i=0; i<count; i++)
		if (store[i].item != self)
			release((void *)store[i].item);
	self->count = 0;
}
/* End of Overrides */


static bool Vector_setVectorSize (void *const _self, UInteger size) {
	struct Vector *self = _self;
	struct Array *arraySelf = _self;
	/* If the size is the same do nothing */
	if ( size == self->capacity ) return 0;
	
	/* if the new size is bigger than before, grow */
	if ( size > self->capacity )
		return __ensureCapacity(self, size);
	else { /* If the new size is smaller, then shrink */
		struct _Bucket *store = getStore(_self);
		UInteger count = arraySelf->count;
		if (size>=count) return 0;

		/* release objets greater thant the new size*/
		for (UInteger i = size; i<count; i++, arraySelf->count-- )
			release((void *)store[i].item);
		
		return __shrink(self, size);
	}
	return 0;
}

static void Vector_setVectorCapacityIncrement(void *const _self, UInteger capacityIncrement) {
	struct Vector *self = _self;
	self->capacityIncrement = capacityIncrement;
}

static void Vector_replaceObjectAtIndexWithObject(void *const _self, UInteger index, void *const other) {
	struct Vector *self = _self;
	if ( index > getCollectionCount(self) ) return;
	
	struct _Bucket *store = getStore(self);
	void *old =  (void *)store[index].item;
	if (old != _self)
		release((void *)old);
	
	if ( other != _self )
		retain(other);
	store[index].item = other;
}

static ObjectRef Vector_popObject(void *const _self) {
	struct Array *const self = _self;
	if ( self->count <= 0 ) return (ObjectRef)NULL;
	ObjectRef o = getObjectAtIndex(_self, 0);
	if ( o != self ) retain(o);
	removeObjectAtIndex(self, 0);
	return o;
}


void initVector() {
	initArray();
	initMutableArray();
	
	if ( ! VectorClass )
		VectorClass = new(MutableArrayClass, "VectorClass", MutableArrayClass, sizeof(struct VectorClass),
						 constructor, VectorClass_constructor, NULL);
	if ( ! Vector )
		Vector = new(VectorClass, "Vector", MutableArray, sizeof(struct Vector),
					constructor, Vector_constructor,
					destructor, Vector_destructor,
					
					/* Overrides */
					copy, Vector_copy,
					equals, Vector_equals,
					 getObjectAtIndex, Vector_getObjectAtIndex,
					 lastObject, Vector_lastObject,
					 firstObject, Vector_firstObject,
					 addObject, Vector_addObject,
					 insertObject, Vector_insertObject,
					 insertObjectAtIndex, Vector_insertObjectAtIndex,
					 removeObjectAtIndex, Vector_removeObjectAtIndex,
					 indexOfObject, Vector_indexOfObject,
					 containsObject, Vector_arrayContainsObject,
					 removeObject, Vector_removeObject,
					 removeFirstObject, Vector_removeFirstObject,
					 removeLastObject, Vector_removeLastObject,
					 removeAllObjects, Vector_removeAllObjects,
					 
					 replaceObjectAtIndexWithObject, Vector_replaceObjectAtIndexWithObject,
					 popObject, Vector_popObject,
					
					/* new */
					 getVectorCapacity, Vector_getVectorCapacity,
					 getVectorCapacityIncrement, Vector_getVectorCapacityIncrement,
					 setVectorCapacityIncrement, Vector_setVectorCapacityIncrement,
					 setVectorSize, Vector_setVectorSize,
					NULL);
	
}

void deallocVector() {
//	free((void *)Vector), Vector = NULL;
//	free((void *)VectorClass), VectorClass = NULL;
	release((void *)Vector), Vector = NULL;
	release((void *)VectorClass), VectorClass = NULL;
	deallocMutableArray();
	deallocArray();
}


/* API */

UInteger getVectorCapacity(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	const struct VectorClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->getVectorCapacity,EINVAL,0);
	return class->getVectorCapacity(self);
}

UInteger getVectorCapacityIncrement(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	const struct VectorClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->getVectorCapacityIncrement,EINVAL,0);
	return class->getVectorCapacityIncrement(self);
}

bool setVectorSize(void *const self, UInteger size) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	const struct VectorClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->setVectorSize,EINVAL,0);
	return class->setVectorSize(self, size);
}

void setVectorCapacityIncrement(void *const self, UInteger capacityIncrement) {
	COAssertNoNullOrBailOut(self,EINVAL);
	const struct VectorClass *const class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->setVectorCapacityIncrement,EINVAL);
	class->setVectorCapacityIncrement(self, capacityIncrement);
}


