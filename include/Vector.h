//
//  Vector.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Vector_h
#define CObjects_Vector_h

CO_DECLARE_CLASS(Vector)

UInteger getVectorCapacity(const void *const self);
UInteger getVectorCapacityIncrement(const void *const self);
void setVectorCapacityIncrement(void *const self, UInteger capacityIncrement);
bool setVectorSize(void *const self, UInteger size);

#endif
