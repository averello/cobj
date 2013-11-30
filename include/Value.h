//
//  Value.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Value_h
#define CObjects_Value_h

extern const void * Value;
extern const void * ValueClass;
typedef void * ValueRef;

void initValue() __attribute__ ((constructor));
void deallocValue() __attribute__ ((destructor));

void *getValuePointer(const void *const self);
void setValuePointer(void *const self, const void *const pointer);

// clenaup CAN be NULL
void setValuePointerCleanup(const void *const self, void (*cleanup)(void *));

#endif
