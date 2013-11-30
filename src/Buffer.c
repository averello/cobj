//
//  Buffer.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

#include <cobj.h>
#include <Buffer.r>

const void * Buffer = NULL;
const void * BufferClass = NULL;


static void * Buffer_constructor (void * _self, va_list * app) {
	struct Buffer *self = super_constructor(Buffer, _self, app);
	
	void *buffer = va_arg(*app, void *);
	if ( buffer == NULL ) return free(self), NULL;
	
	size_t length = va_arg(*app, size_t);
	if ( length == 0 ) return free(self), NULL;
	
	self->buffer = calloc(1, length);
	assert( self->buffer != NULL );
	if ( self->buffer == NULL ) return free(self), NULL;
	
	memcpy((void *)self->buffer, buffer, length);
	self->length = length;
	
	return self;
}

static void * Buffer_destructor (void * _self) {
	struct Buffer *self = super_destructor(Buffer, _self);
	free((void *)self->buffer), self->buffer = NULL;
	return self;
}

static void * BufferClass_constructor (void * _self, va_list *app) {
	struct BufferClass * self = super_constructor(BufferClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getBufferBytes )
			* (voidf *) & self->getBufferBytes = method;
		else if (selector == (voidf) getBufferBytesOfLength )
			* (voidf *) & self->getBufferBytesOfLength = method;
		else if (selector == (voidf) getBufferBytesInRange )
			* (voidf *) & self->getBufferBytesInRange = method;
		else if (selector == (voidf) getBufferLength )
			* (voidf *) & self->getBufferLength = method;
	}
	va_end(ap);
	return self;
}

static void * Buffer_copy (const void * const _self) {
	const struct Buffer *self = _self;
	return new(Buffer, getBufferBytes(self), getBufferLength(self), NULL);
}

static int Buffer_equals (const void * const _self, const void *const _other) {
	const struct Buffer *self = _self;
	const struct Buffer *other = _other;
	const struct Classs *const _super = (const struct Classs *const )super(_self);
	int result = _super->equals(_self, _other);
	if ( ! result )
		result = (self->length == other->length) && (memcmp(self->buffer, other->buffer, self->length) == 0);
	return result;
}

static const void * Buffer_getBufferBytes (const void *const _self) {
	const struct Buffer *self = _self;
	return self->buffer;
}

static void Buffer_getBufferBytesOfLength (const void *const _self, void *restrict buffer, size_t length) {
	const struct Buffer *self = _self;
	if (buffer == NULL) return;
	if ( length > self->length ) length = self->length;
	memcpy(buffer, self->buffer, length);
}

static void Buffer_getBufferBytesInRange (const void *const _self, void *restrict buffer, SRange range) {
	const struct Buffer *self = _self;
	if (buffer == NULL) return;
	if ( range.location >= self->length || range.length > self->length ) return;
	memcpy(buffer, ((char *)self->buffer) + range.location, range.length);
}

static size_t Buffer_getBufferLength (const void *const _self) {
	const struct Buffer *self = _self;
	return self->length;
}

void initBuffer() {
	if ( ! BufferClass )
		BufferClass = new(Class, "BufferClass", Class, sizeof(struct BufferClass),
						  constructor, BufferClass_constructor, NULL);
	if ( ! Buffer )
		Buffer = new(BufferClass, "Buffer", Object, sizeof(struct Buffer),
					 constructor, Buffer_constructor,
					 destructor, Buffer_destructor,
					 
					 /* Oveerrides */
					 copy, Buffer_copy,
					 equals, Buffer_equals,
					 
					 /* new */
					 getBufferBytes, Buffer_getBufferBytes,
					 getBufferBytesOfLength, Buffer_getBufferBytesOfLength,
					 getBufferBytesInRange, Buffer_getBufferBytesInRange,
					 getBufferLength, Buffer_getBufferLength,
					 NULL);
}

void deallocBuffer() {
	free((void *)Buffer), Buffer = NULL;
	free((void *)BufferClass), BufferClass = NULL;
}



/* API */

const void *getBufferBytes(const void *const self) {
	assert( self != NULL );
	const struct BufferClass *const class = classOf(self);
	assert( class != NULL && class->getBufferBytes != NULL );
	return class->getBufferBytes(self);
}

void getBufferBytesOfLength(const void *const self, void *restrict buffer, size_t length) {
	assert( self != NULL );
	assert( buffer != NULL );
	const struct BufferClass *const class = classOf(self);
	assert( class != NULL && class->getBufferBytesOfLength != NULL );
	class->getBufferBytesOfLength(self, buffer, length);
}

void getBufferBytesInRange(const void *const self, void *restrict buffer, SRange range) {
	assert( self != NULL );
	assert( buffer != NULL );
	const struct BufferClass *const class = classOf(self);
	assert( class != NULL && class->getBufferBytesInRange != NULL );
	class->getBufferBytesInRange(self, buffer, range);
}

size_t getBufferLength(const void *const self) {
	assert( self != NULL );
	const struct BufferClass *const class = classOf(self);
	assert( class != NULL && class->getBufferLength != NULL );
	return class->getBufferLength(self);
}
