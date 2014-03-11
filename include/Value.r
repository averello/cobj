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

CO_BEGIN_CLASS_TYPE_DECL(Value,Object)
	const void * pointer;
	voidf cleanup;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(ValueClass,Classs)
	void * ( *getValuePointer) (const void *const self);
	void ( *setValuePointer) (void *const self, const void *const pointer);
	void ( *setValuePointerCleanup )(const void *const self, void (*cleanup)(void *));
CO_END_CLASS_DECL

#endif
