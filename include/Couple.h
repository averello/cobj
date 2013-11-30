//
//  Couple.h
//  CObjects
//
//  Created by averello on 3/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Couple_h
#define CObjects_Couple_h

extern const void * Couple;
extern const void * CoupleClass;
typedef void * CoupleRef;

void initCouple() __attribute__ ((constructor));
void deallocCouple() __attribute__ ((destructor));

void * getKey(const void * const self);
void * getValue(const void * const self);
void setKey(void * const self, void * const key);
void setValue(void * const self, void * const value);


#endif
