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

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

#include <compilerDefs.h>
#include <Object.h>
#include <Object.r>
#include <new.h>
#include <StringObject.h>
#undef retain
#undef release


void * constructor(void * self, va_list * app) {
	if ( self == NULL ) return errno = EINVAL, NULL;
	const struct Class *class = classOf(self);
	assert(class->constructor != NULL);
	if ( class->constructor == NULL ) return errno = ENOTSUP, self;
	return class->constructor(self, app);
}

void * destructor(void * self) {
	if ( self == NULL ) return errno = EINVAL, NULL;
	const struct Class *class = classOf(self);
	assert(class->destructor != NULL);
	if ( class->destructor == NULL ) return errno = ENOTSUP, self;
	return class->destructor(self);
}

void * super_constructor(const void *const class, void * self, va_list * app) {
	if ( self == NULL ) return errno = EINVAL, NULL;
	assert( self != NULL );
	if ( class == NULL ) return errno = EINVAL, NULL;
	assert( class != NULL );
	const struct Class * _superclass = superclass(class);
	assert(self != NULL && _superclass->constructor != NULL);
	if ( _superclass->constructor == NULL ) return errno = ENOTSUP, self;
	return _superclass->constructor(self, app);
}

void * super_destructor(const void *const class, void * self) {
	if ( self == NULL ) return errno = EINVAL, NULL;
	assert( self != NULL );
	if ( class == NULL ) return errno = EINVAL, NULL;
	assert( class != NULL );
	
	const struct Class * _superclass = superclass(class);
	assert(self != NULL && _superclass->destructor != NULL);
	if ( _superclass->destructor == NULL ) return errno = ENOTSUP, self;
	return _superclass->destructor(self);
}


//***********//


void * Object_constructor (void * _self, va_list * app) {
	struct Object *self = _self;
	MEMORY_MANAGEMENT_INITIALIZE(self);
	memory_management_attributes_set_dealloc_function(self, delete);
//	pthread_mutex_init(&(self->guardian), NULL);
//	self->retainCount = 1;
	return self;
}

void * Object_destructor (void * _self) {
//	struct Object *self = _self;
	/*
	 * Normally is not possible that the mutex is locked when this function is called
	 */
//	if ( pthread_mutex_destroy(&(self->guardian)) != 0 )
//		if	(errno == EBUSY)
//			if ( (pthread_mutex_unlock(&(self->guardian)) != 0) )
//				if ( errno != EINVAL )
//					pthread_mutex_destroy(&(self->guardian));
//	return _self;
	return _self;
}

void * Object_copy (const void * const _self) {
	return new(Object);
}

int Object_equals (const void * const _self, const void *const _other) {
	const struct Object *const self = _self;
	const struct Object *const other = _other;
	return (self == other);
}

//GCC_DIAG_ON(no-pointer-to-int-cast)
int Object_hash (const void *const _self) {
	const struct Object *const self = _self;
	int prime = 31;
	int result = 1;
	result = prime * result + (int)self;
	return result;
}
//GCC_DIAG_OFF(no-pointer-to-int-cast)

void * Object_retain (void * const _self) {
	struct Object *const self = _self;
	MEMORY_MANAGEMENT_RETAIN(self);
//	if ( pthread_mutex_lock(&(self->guardian)) != 0 ) {
//		assert(0);
//	}
//	self->retainCount++;
//	if ( pthread_mutex_unlock(&(self->guardian)) != 0 ) {
//		assert(0);
//	}
	return self;
}

void Object_release (void * const _self) {
	struct Object *const self = _self;
	MEMORY_MANAGEMENT_RELEASE(self);
//	if ( pthread_mutex_lock(&(self->guardian)) != 0 ) {
//		assert(0);
//	}
//	self->retainCount--;
//	if ( self->retainCount == 0) {
//		if ( pthread_mutex_unlock(&(self->guardian)) != 0 ) {
//			assert(0);
//		}
//		delete(self);
//	} else
//		if ( pthread_mutex_unlock(&(self->guardian))  != 0 ) {
//			assert(0);
//		}
}

unsigned long Object_retainCount (const void * const _self) {
	const struct Object *const self = _self;
	return MEMORY_MANAGEMENT_GET_RETAIN_COUNT(self);
//	return self->retainCount;
}

StringRef Object_copyDescription (const void * const _self) {
	const struct Object *const self = _self;
	const struct Class *class = classOf(self);
	StringRef copyDescription = newStringWithFormat(String, "<%s 0x%x retainCount:%ul>", class->class_name, self, retainCount(self), NULL);
	return copyDescription;
}


/* */

const char *getClassName(const void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, NULL;
	
	const struct Class *class = classOf(self);
	assert( class != NULL );
	if ( class == NULL ) return errno = ENOTSUP, NULL;
	return class->class_name;
}

void * copy(const void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, NULL;
	
	const struct Class *class = classOf(self);
	assert(class->copy != NULL);
	if ( class->copy == NULL ) return errno = ENOTSUP, NULL;
	
	return class->copy(self);
}

int equals(const void *const self, const void *const other) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, 0;
	
	const struct Class *class = classOf(self);
	assert(class->equals != NULL);
	if ( class->equals == NULL ) return errno = ENOTSUP, 0;
	
	return class->equals(self, other);
}

int hash (const void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, -1;
	
	const struct Class *class = classOf(self);
	assert(class->hash != NULL);
	if ( class->hash == NULL ) return errno = ENOTSUP, -1;
	
	return class->hash(self);
}

void * retain (void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, NULL;
	
	const struct Class *class = classOf(self);
	assert(class->retain != NULL);
	if ( class->retain == NULL ) return errno = ENOTSUP, NULL;
	
	return class->retain(self);
}

void release (void *const self) {
	assert( self != NULL);
	if ( self == NULL ) { errno = EINVAL; return; };
	
	const struct Class *class = classOf(self);
	assert(class->release != NULL);
	if ( class->release == NULL ) { errno = ENOTSUP; return; }
	
	class->release(self);
}

unsigned long retainCount (const void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, 0;
	
	const struct Class *class = classOf(self);
	assert(class->retainCount != NULL);
	if ( class->retainCount == NULL ) return errno = ENOTSUP, 0;
	
	return class->retainCount(self);
}

int instanceOf (const void * const self, const void *const _class) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, -1;
	
	assert( _class != NULL);
	if ( _class == NULL ) return errno = EINVAL, -1;
	
	const struct Class *class = classOf(self);
	assert(class != NULL);
	if ( class == NULL ) return errno = EINVAL, -1;
	
	/* Get the type */
	const struct Class *const mclass = _class;
	return ( strcmp(class->class_name, mclass->class_name) == 0 );
}

int isSubclassOf (const void *const self, const void * const _class) {
	int result = instanceOf(self, _class);
	if ( result == 0)
		result = isSubclassOf(superclass(self), _class);
	return result;
}

size_t sizeOf(const void *const self) {
	assert( self != NULL);
	if ( self == NULL ) return errno = EINVAL, 0;
	
	const struct Class *const class = classOf(self);
	assert(class != NULL);
	if ( class == NULL ) return errno = EINVAL -1;
	
	return class->size;
}

const void * classOf(const void * const _self) {
	assert( _self != NULL );
	if ( _self == NULL ) return errno = EINVAL, NULL;
	
	const struct Object *const self = _self;
	assert( self->class != NULL );
	if ( self->class == NULL ) return errno = EINVAL, NULL;
	
	return (const void *)self->class;
}

const void * superclass (const void *const _class) {
	assert( _class != NULL );
	if ( _class == NULL ) return errno = EINVAL, NULL;
	
	const struct Class * self = _class;
	assert( self->super != NULL);
	if ( self->super == NULL ) return errno = EINVAL, NULL;
	
	return self->super;
}

const void * super(const void *const _self) {
	assert( _self != NULL );
	if ( _self == NULL ) return errno = EINVAL, NULL;
	
	const struct Class * self = classOf(_self);
	assert( self->super != NULL);
	if ( self->super == NULL ) return errno = EINVAL, NULL;
	
	return (const void *)self->super;
}

void * copyDescription (const void *const self) {
	assert(self!=NULL);
	if ( self == NULL ) return errno = EINVAL, NULL;
	
	const struct Class *class = classOf(self);
	assert(class->copyDescription != NULL);
	if ( class->copyDescription == NULL ) return errno = ENOTSUP, NULL;
	
	return class->copyDescription(self);
}



