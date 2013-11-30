//
//  Vector.r
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Vector_r
#define CObjects_Vector_r

#include <cobj.h>
#include <Object.r>
#include <MutableArray.r>

struct Vector {
	const struct MutableArray isa;
	size_t capacityIncrement;
	size_t capacity;
};

struct VectorClass {
	const struct MutableArrayClass isa;
	size_t ( * getVectorCapacity ) (const void *const self);
	size_t ( * getVectorCapacityIncrement ) (const void *const self);
	int ( * setVectorSize ) (void *const self, size_t size);
	void ( * setVectorCapacityIncrement )(void *const self, size_t capacityIncrement);
};

#endif
