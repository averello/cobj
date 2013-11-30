//
//  Value.r
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Value_r
#define CObjects_Value_r

#include <cobj.h>
#include <Object.r>

typedef void* (*voidf) (void *);

struct Value {
	const struct Object isa;
	const void * pointer;
	voidf cleanup;
};

struct ValueClass {
	const struct Class isa;
	void * ( *getValuePointer) (const void *const self);
	void ( *setValuePointer) (void *const self, const void *const pointer);
	void ( *setValuePointerCleanup )(const void *const self, void (*cleanup)(void *));
};

#endif
