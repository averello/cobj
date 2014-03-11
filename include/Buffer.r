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

CO_BEGIN_CLASS_TYPE_DECL(Buffer,Object)
	const void * buffer;
	UInteger length;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(BufferClass,Classs)
	const void * ( * getBufferBytes ) (const void *const self);
	void ( * getBufferBytesOfLength ) (const void *const self, void *restrict buffer, UInteger length);
	void ( * getBufferBytesInRange ) (const void *const self, void *restrict buffer, Range range);
	UInteger ( * getBufferLength ) (const void *const self);
CO_END_CLASS_DECL

#endif
