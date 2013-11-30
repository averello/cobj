//
//  Value.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

#include <cobj.h>
#include <Value.r>

const void * Value = NULL;
const void * ValueClass = NULL;


static void * Value_constructor (void * _self, va_list * app) {
	struct Value *self = super_constructor(Value, _self, app);
	
	void *pointer = va_arg(*app, void *);
	if ( pointer == NULL ) return free(self), NULL;
	self->pointer = pointer;
	
	voidf cleanup = va_arg(*app, voidf);
	if ( cleanup != NULL ) self->cleanup = cleanup;
	return self;
}

static void * Value_destructor (void * _self) {
	struct Value *self = super_destructor(Value, _self);
	if ( self->cleanup != NULL ) self->cleanup((void *)self->pointer);
	return self;
}

static void * ValueClass_constructor (void * _self, va_list *app) {
	struct ValueClass * self = super_constructor(ValueClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getValuePointer )
			* (voidf *) & self->getValuePointer = method;
		else if (selector == (voidf) setValuePointer )
			* (voidf *) & self->setValuePointer = method;
		else if (selector == (voidf) setValuePointerCleanup )
			* (voidf *) & self->setValuePointerCleanup = method;
	}
	va_end(ap);
	return self;
}

static void Value_setValuePointer(void *const _self, const void *const pointer) {
	struct Value *const self = _self;
	self->pointer = pointer;
}

static void * Value_getValuePointer (const void *const _self) {
	const struct Value *const self = _self;
	return (void *)self->pointer;
}

static void * Value_copy (const void * const _self) {
	const struct Value *self = _self;
	return new(Value, getValuePointer(self), NULL);
}

static int Value_equals (const void * const _self, const void *const _other) {
	const struct Value *self = _self;
	const struct Value *other = _other;
	const struct Class *const _super = (const struct Class *const )super(_self);
	
	return ( _super->equals(_self, _other) || (self->pointer == other->pointer) );
}

static int Value_hash (const void *const _self) {
	const struct Value *const self = _self;
	int prime = 31;
	int result = 1;
//	result = prime * result + (int)self;
	result = prime * result + (int)self->pointer;
	return result;
}

static void Value_setValuePointerCleanup (void *const _self, voidf cleanup) {
	struct Value *const self = _self;
	self->cleanup = cleanup;
}


void initValue() {
	if ( ! ValueClass )
		ValueClass = new(Class, "ValueClass", Class, sizeof(struct ValueClass),
						 constructor, ValueClass_constructor, NULL);
	if ( ! Value )
		Value = new(ValueClass, "Value", Object, sizeof(struct Value),
					constructor, Value_constructor,
					destructor, Value_destructor,
					
					/* Oveerrides */
					copy, Value_copy,
					equals, Value_equals,
					hash, Value_hash,
					
					/* new */
					getValuePointer, Value_getValuePointer,
					setValuePointer, Value_setValuePointer,
					setValuePointerCleanup, Value_setValuePointerCleanup,
					NULL);
		
}

void deallocValue() {
	free((void *)Value), Value = NULL;
	free((void *)ValueClass), ValueClass = NULL;
}


/* API */

void *getValuePointer(const void *const self) {
	assert( self != NULL );
	const struct ValueClass *const class = classOf(self);
	assert( class != NULL && class->getValuePointer != NULL );
	return class->getValuePointer(self);
}

void setValuePointer(void *const self, const void *const pointer) {
	assert( self != NULL );
	assert( pointer != NULL );
	const struct ValueClass *const class = classOf(self);
	assert( class != NULL && class->setValuePointer != NULL );
	class->setValuePointer(self, pointer);
}

void setValuePointerCleanup(const void *const self, void (*cleanup)(void *)) {
	assert( self != NULL );
	const struct ValueClass *const class = classOf(self);
	assert( class != NULL && class->setValuePointerCleanup != NULL );
	class->setValuePointerCleanup(self, cleanup);
}


