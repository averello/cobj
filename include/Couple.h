//
//  Couple.h
//  CObjects
//
//  Created by averello on 3/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Couple_h
#define CObjects_Couple_h

CO_DECLARE_CLASS(Couple)

void * getKey(const void * const self);
void * getValue(const void * const self);
void setKey(void * const self, void * const key);
void setValue(void * const self, void * const value);


#endif
