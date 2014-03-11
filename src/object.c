//
//  object.c
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <errno.h>
#include <memory_management/memory_management.h>


#include <coassert.h>
#include <compilerDefs.h>
#include <Object.h>
#include <Object.r>
#include <new.h>
#include <StringObject.h>
#include <AutoreleasePool.h>
#undef retain
#undef release


void * constructor(void * self, va_list * app) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,ENOTSUP,NULL);
	return class->constructor(self, app);
}

void * destructor(void * self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->destructor,ENOTSUP,NULL);
	return class->destructor(self);
}

void * super_constructor(const void *const class, void * self, va_list * app) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	
	const struct Classs * _superclass = superclass(class);
	COAssertNoNullOrReturn(_superclass,EINVAL,NULL);
	COAssertNoNullOrReturn(_superclass->constructor,ENOTSUP,NULL);
	return _superclass->constructor(self, app);
}

void * super_destructor(const void *const class, void * self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	
	const struct Classs * _superclass = superclass(class);
	COAssertNoNullOrReturn(_superclass,EINVAL,NULL);
	COAssertNoNullOrReturn(_superclass->destructor,ENOTSUP,NULL);
	return _superclass->destructor(self);
}


//***********//


void * Object_constructor (void * _self, va_list * app) {
	struct Object *self = _self;
	MEMORY_MANAGEMENT_ATTRIBUTE_SET_DEALLOC_FUNCTION(self, delete);
	return self;
}

void * Object_destructor (void * _self) {
	return _self;
}

void * Object_copy (const void * const _self) {
	return new(Object);
}

bool Object_equals (const void * const _self, const void *const _other) {
	const struct Object *const self = _self;
	const struct Object *const other = _other;
	return (bool)(self == other);
}

//GCC_DIAG_ON(no-pointer-to-int-cast)
UInteger Object_hash (const void *const _self) {
	const struct Object *const self = _self;
	UInteger prime = 31;
	UInteger result = 1;
	result = prime * result + (UInteger)self;
	return result;
}
//GCC_DIAG_OFF(no-pointer-to-int-cast)

void * Object_retain (void * const _self) {
	struct Object *const self = _self;
	MEMORY_MANAGEMENT_RETAIN(self);
	return self;
}

void Object_release (void * const _self) {
	struct Object *const self = _self;
	MEMORY_MANAGEMENT_RELEASE(self);
}

UInteger Object_retainCount (const void * const _self) {
	const struct Object *const self = _self;
	return MEMORY_MANAGEMENT_GET_RETAIN_COUNT(self);
}

void * Object_autorelease (void * _self) {
	return _self;
}

StringRef Object_copyDescription (const void * const _self) {
	const struct Object *const self = _self;
	const struct Classs *class = classOf(self);
	StringRef copyDescription = newStringWithFormat(String, "<%s %p retainCount:%ul>", class->class_name, self, retainCount(self), NULL);
	return copyDescription;
}


/* */

const char *getClassName(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	return class->class_name;
}

void * copy(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->copy,ENOTSUP,NULL);
	
	return class->copy(self);
}

bool equals(const void *const self, const void *const other) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	COAssertNoNullOrReturn(class->equals,ENOTSUP,-1);
	
	return class->equals(self, other);
}

UInteger hash (const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->hash,ENOTSUP,0);
	
	return class->hash(self);
}

void * retain (void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->retain,ENOTSUP,NULL);
	
	return class->retain(self);
}

void release (void *const self) {
	COAssertNoNullOrBailOut(self,EINVAL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->release,ENOTSUP);
	
	class->release(self);
}

UInteger retainCount (const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->retainCount,ENOTSUP,0);
	
	return class->retainCount(self);
}

bool instanceOf (const void * const self, const void *const _class) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	COAssertNoNullOrReturn(_class,EINVAL,-1);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	
	/* Get the type */
	const struct Classs *const mclass = _class;
	return ( strcmp(class->class_name, mclass->class_name) == 0 );
}

bool isSubclassOf (const void *const self, const void * const _class) {
	bool result = instanceOf(self, _class);
	if ( !result )
		result = isSubclassOf(superclass(self), _class);
	return result;
}

UInteger sizeOf(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	
	const struct Classs *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	
	return class->size;
}

const void * classOf(const void * const _self) {
	COAssertNoNullOrReturn(_self,EINVAL,NULL);
	
	const struct Object *const self = _self;
	COAssertNoNullOrReturn(self->class,EINVAL,NULL);
	return (const void *)self->class;
}

const void * superclass (const void *const _class) {
	COAssertNoNullOrReturn(_class,EINVAL,NULL);
	
	const struct Classs * self = _class;
	COAssertNoNullOrReturn(self->super,EINVAL,NULL);
	
	return self->super;
}

const void * super(const void *const _self) {
	COAssertNoNullOrReturn(_self,EINVAL,NULL);
	
	const struct Classs * self = classOf(_self);
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	COAssertNoNullOrReturn(self->super,ENOTSUP,NULL);
	
	return (const void *)self->super;
}

void * copyDescription (const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	
	const struct Classs *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->copyDescription,ENOTSUP,NULL);
	
	return class->copyDescription(self);
}


void *autorelease (void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	AutoreleasePoolAddObject(self);
	return self;
//	assert(self!=NULL);
//	if ( self == NULL ) return errno = EINVAL, NULL;
//	
//	const struct Classs *class = classOf(self);
//	assert(class->autorelease != NULL);
//	if ( class->autorelease == NULL ) return errno = ENOTSUP, NULL;
//	
//	return class->autorelease(self);
}


