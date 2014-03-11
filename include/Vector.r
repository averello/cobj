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
	uint32_t capacityIncrement;
	uint64_t capacity;
};

struct VectorClass {
	const struct MutableArrayClass isa;
	uint64_t ( * getVectorCapacity ) (const void *const self);
	uint32_t ( * getVectorCapacityIncrement ) (const void *const self);
	int ( * setVectorSize ) (void *const self, uint64_t size);
	void ( * setVectorCapacityIncrement )(void *const self, uint32_t capacityIncrement);
};

#endif
