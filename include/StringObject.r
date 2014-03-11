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
#include <stdint.h>

struct String {
	const struct Object isa;
	const void *text;
	uint64_t length;
	int _hash;
};

struct StringClass {
	const struct Classs isa;
	const char * (* getStringText)(const void *const self);
	uint64_t (* getStringLength)(const void *const self);
	
	int (* characterAtIndex) (const void *const self, void *const character, uint64_t index);
	int (* getCharactersInRange) (const void *const self, void *const buffer, CORange range);

	StringRef (* copyStringByAppendingString)(const void *restrict const self, const void *restrict const other);
	StringRef (* newStringWithFormat)(const void *const _class, const char *format, va_list *ap);
	
	SComparisonResult ( *compare) (const void *const self, const void *const other);
	SComparisonResult ( *compareWithOptions) (const void *const self, const void *const other, SStringComparingOptions options);
	
	StringRef ( * copyStringByTrimmingSpaces ) (const void *const self);
};


#endif
