//
//  Vector.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Vector_h
#define CObjects_Vector_h

extern const void * Vector;
extern const void * VectorClass;
typedef void * VectorRef;

void initVector() __attribute__ ((constructor));
void deallocVector() __attribute__ ((destructor));

uint64_t getVectorCapacity(const void *const self);
uint32_t getVectorCapacityIncrement(const void *const self);
void setVectorCapacityIncrement(void *const self, uint32_t capacityIncrement);
int setVectorSize(void *const self, uint64_t size);

#endif
