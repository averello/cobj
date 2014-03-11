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
	UInteger capacityIncrement;
	UInteger capacity;
};

struct VectorClass {
	const struct MutableArrayClass isa;
	UInteger ( * getVectorCapacity ) (const void *const self);
	UInteger ( * getVectorCapacityIncrement ) (const void *const self);
	bool ( * setVectorSize ) (void *const self, UInteger size);
	void ( * setVectorCapacityIncrement )(void *const self, UInteger capacityIncrement);
};

#endif
