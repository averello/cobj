//
//  String.r
//  CObjects
//
//  Created by George Boumis on 10/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_String_r
#define CObjects_String_r

#include <Object.h>
#include <Object.r>
#include <StringObject.h>
#include <codefinitions.h>

CO_BEGIN_CLASS_TYPE_DECL(String, Object)
	const void *text;
	UInteger length;
	UInteger _hash;
CO_END_CLASS_TYPE_DECL



CO_BEGIN_CLASS_DECL(StringClass,Classs)
	const char * (* getStringText)(const void *const self);
	UInteger (* getStringLength)(const void *const self);
	
	int (* characterAtIndex) (const void *const self, void *const character, UInteger index);
	int (* getCharactersInRange) (const void *const self, void *const buffer, Range range);

	StringRef (* copyStringByAppendingString)(const void *restrict const self, const void *restrict const other);
	StringRef (* newStringWithFormat)(const void *const _class, const char *format, va_list *ap);
	
	SComparisonResult ( *compare) (const void *const self, const void *const other);
	SComparisonResult ( *compareWithOptions) (const void *const self, const void *const other, SStringComparingOptions options);
	
	StringRef ( * copyStringByTrimmingSpaces ) (const void *const self);
CO_END_CLASS_DECL


#endif
