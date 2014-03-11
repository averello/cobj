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

struct MutableString {
	const struct String isa;
	off_t offset;
	uint64_t capacity;
};

struct MutableStringClass {
	const struct StringClass isa;
	void ( * appendString ) (void *const self, const void *const other);
	void ( * appendFormat ) (void *const self, char *format, va_list *app);
	void ( * setString ) (void *const self, const void *const other);
	void ( * setMutableStringLength ) (void *const self, size_t capacity);
	
	int ( * insertStringAtMutableStringIndex ) (void *const self, const void *const other, unsigned long index);
	int ( * deleteMutableStringCharactersInRange ) (void *const self, SRange range);

};

#endif
