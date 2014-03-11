//
//  Couple.r
//  CObjects
//
//  Created by averello on 3/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Couple_r
#define CObjects_Couple_r

#include <cobj.h>
#include <Object.r>


CO_BEGIN_CLASS_TYPE_DECL(Couple,Object)
	void * key;
	void * value;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(CoupleClass,Classs)
	void * ( *getKey ) (const void * const self);
	void * ( *getValue ) (const void * const self);
	
	void ( *setKey ) (void * const self, void * const key);
	void ( *setValue ) (void * const self, void * const value);
CO_END_CLASS_DECL

#endif
