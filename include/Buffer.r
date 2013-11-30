//
//  Buffer.r
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Buffer_r
#define CObjects_Buffer_r

#include <cobj.h>
#include <Object.r>

struct Buffer {
	const struct Object isa;
	const void * buffer;
	size_t length;
};

struct BufferClass {
	const struct Class isa;

	const void * ( * getBufferBytes ) (const void *const self);
	void ( * getBufferBytesOfLength ) (const void *const self, void *restrict buffer, size_t length);
	void ( * getBufferBytesInRange ) (const void *const self, void *restrict buffer, SRange range);
	size_t ( * getBufferLength ) (const void *const self);
};

#endif
