//
//  MutableString.r
//  CObjects
//
//  Created by averello on 6/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableString_r
#define CObjects_MutableString_r

#include <cobj.h>
#include <Object.r>
#include <StringObject.r>

CO_BEGIN_CLASS_TYPE_DECL(MutableString,String)
	off_t offset;
	UInteger capacity;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(MutableStringClass,StringClass)
	void ( * appendString ) (void *const self, const void *const other);
	void ( * appendFormat ) (void *const self, char *format, va_list *app);
	void ( * setString ) (void *const self, const void *const other);
	void ( * setMutableStringLength ) (void *const self, UInteger capacity);
	
	int ( * insertStringAtMutableStringIndex ) (void *const self, const void *const other, UInteger index);
	int ( * deleteMutableStringCharactersInRange ) (void *const self, Range range);
CO_END_CLASS_DECL

#endif
