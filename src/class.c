//
//  class.c
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <coassert.h>
#include <errno.h>

#include <Object.h>
#include <Object.r>
#include <new.h>


static void * Class_constructor (void * _self, va_list * app) {
	struct Classs * self = _self;
	
	self->class_name = va_arg( *app, const char *);
	self->super = va_arg( *app, struct Classs *);
	self->size = va_arg( *app, size_t);
	
	assert(self->super != NULL);
	const size_t offset	 = sizeof(struct Object) + sizeof(char *) + sizeof(struct Class *) + sizeof(size_t);
	memcpy((char *)self  + offset, (char *)(self->super) + offset, sizeOf((void *)self->super) - offset);
	{
		typedef void (*voidf) ();
		voidf selector = NULL;
		va_list ap;
		va_copy(ap, *app);
		
		while ( (selector = va_arg(ap, voidf)) ) {
			voidf method = va_arg(ap, voidf);
			
			if ( selector == (voidf) constructor )
				* (voidf *) & self->constructor = method;
			else if ( selector == (voidf) destructor )
				* (voidf *) & self->destructor = method;
			
			else if ( selector == (voidf) copy )
				* (voidf *) & self->copy = method;
			else if ( selector == (voidf) equals )
				* (voidf *) & self->equals = method;
			
			else if ( selector == (voidf) hash )
				* (voidf *) & self->hash = method;
			
			else if ( selector == (voidf) retain )
				* (voidf *) & self->retain = method;
			else if ( selector == (voidf) release )
				* (voidf *) & self->release = method;
			else if ( selector == (voidf) retainCount )
				* (voidf *) & self->retainCount = method;
			
			else if ( selector == (voidf) copyDescription )
				* (voidf *) & self->copyDescription = method;
			
			else if ( selector == (voidf) autorelease )
				* (voidf *) & self->autorelease = method;
		}
		va_end(ap);
	}
	
	return self;
}

static void * Class_destructor (void * _self) {
	return NULL;
}

static void * Class_copy (const void *const _self) {
	return NULL;
}

static int Class_hash (const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	const struct Classs *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	const struct Classs *const _superclass = superclass(self);
	COAssertNoNullOrReturn(_superclass,EINVAL,-1);
	COAssertNoNullOrReturn(_superclass->hash,EINVAL,-1);
	
	
	int prime = 31;
	int result = _superclass->hash(self);
	result = prime * result  + (int)strlen(class->class_name);
	result = prime * result + (int)class->size;
	return result;
}
static void * Class_retain (void *const _self) {
	return NULL;
}

static void Class_release (void *const _self) {
}

static void *Class_autorelease (void *const _self) {
	return NULL;
}

static const struct Classs object [] = {
	{	{object+1},
		"Object",
		object,
		sizeof(struct Object),
		Object_constructor,
		Object_destructor,
		Object_copy,
		Object_equals,
		Object_hash,
		Object_copyDescription,
		Object_retain,
		Object_release,
		Object_retainCount,
		Object_autorelease,
	},
	{	{object+1},
		"Class",
		object,
		sizeof(struct Classs),
		Class_constructor,
		Class_destructor,
		Class_copy,
		Object_equals,
		Class_hash,
		Object_copyDescription,
		Class_retain,
		Object_release,
		Object_retainCount,
		Class_autorelease,
	}
};

const void *const Object = (const void * const)(object);
const void *const Class = (const void * const )(object+1);



