//
//  String.c
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

#include <coassert.h>

#include <StringObject.h>
#include <StringObject.r>
#include <new.h>
#include <Object.h>
#include <Object.r>


extern int errno;

static void * String_constructor (void * _self, va_list * app) {
	struct String *self = super_constructor(String, _self, app);
	
	const char * text = va_arg(* app, const char *);
	if (NULL==text) {
		self->length = 0;
		return self;
	}
	self->text = strdup(text);
	assert(self->text != NULL);
	if ( self->text == NULL ) return free(self), NULL;
	self->length = strlen(self->text);
	self->_hash = 0;
	return self;
}

static void * StringClass_constructor (void * _self, va_list *app) {
	struct StringClass * self = super_constructor(StringClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getStringLength)
			* (voidf *) & self->getStringLength = method;
		else if (selector == (voidf) getStringText )
			* (voidf *) & self->getStringText = method;
		
		else if (selector == (voidf) characterAtIndex )
			* (voidf *) & self->characterAtIndex = method;
		else if (selector == (voidf) getCharactersInRange )
			* (voidf *) & self->getCharactersInRange = method;
		
		else if (selector == (voidf) copyStringByAppendingString)
			* (voidf *) & self->copyStringByAppendingString = method;
		else if (selector == (voidf) newStringWithFormat )
			* (voidf *) & self->newStringWithFormat = method;
		
		else if (selector == (voidf) compare )
			* (voidf *) & self->compare = method;
		else if (selector == (voidf) compareWithOptions )
			* (voidf *) & self->compareWithOptions = method;
		
		else if (selector == (voidf) copyStringByTrimmingSpaces )
			* (voidf *) & self->copyStringByTrimmingSpaces = method;
	}
	va_end(ap);
	return self;
}

static void * String_destructor (void * _self) {
	struct String *self = super_destructor(String, _self);
	free((char *)self->text), self->text = NULL;
	return self;
}

static void * String_copy (const void *const _self) {
	const struct String *self = _self;
	StringRef _copy = new(String, self->text, NULL);
	struct String *copy = _copy;
	copy->_hash = self->_hash;
	return _copy;
}

static int String_equals (const void * const _self, const void *const _other) {
	const struct String *self = _self;
	const struct String *other = _other;
	const struct Classs *const _super = (const struct Classs *const )super(_self);
	
	
	int result = _super->equals(_self, _other);
	if ( ! result && (getStringLength(self) == getStringLength(other)) && self->text && other->text)
		result = (strcmp(self->text, other->text) == 0);
	return result;
}

static int String_hash(const void *const _self) {
	const struct String *self = _self;
	if (self->_hash == 0) {
//		int h = self->_hash;
//        if (h == 0 && getStringLength(self)  > 0) {
//            int off = 0;
//            const char *val = getStringText(self);
//            long len = getStringLength(self);
//            for (long i = 0; i <  len; i++)
//                h = 31*h + val[off++];
//			((struct String *)self)->_hash = h;
//        }
		
		int hash = 0;
		const char *restrict text = getStringText(self);
		for(long i = 0; i < self->length; text++, i++)
			hash =  (*text) + (hash << 6) + (hash << 16) - hash;
		((struct String *)self)->_hash = hash;
	}
	return self->_hash;
}

static StringRef String_newStringWithFormat(const void *const _class, const char *const format, va_list *ap) {
	StringRef newString = NULL;
	char buffer[BUFSIZ];
	memset(buffer, '\0', BUFSIZ);
	
	va_list copy;
	va_copy(copy, *ap);
	
	int totalWritten = vsnprintf(buffer, BUFSIZ, format, *ap);
	if (totalWritten>= BUFSIZ) {
		char *newBuffer = calloc(totalWritten+1, sizeof(char));
		assert(newBuffer != NULL);
		vsnprintf(newBuffer, totalWritten+1, format, copy);
		newString = new(_class, newBuffer, NULL);
		free(newBuffer);
	}
	else
		newString = new(_class, buffer, NULL);
	va_end(copy);
	return newString;
}

static StringRef String_copyStringByAppendingString(const void *restrict const _self, const void *restrict const _other) {
	const struct String *self = _self;
	const struct String *other = _other;
	return newStringWithFormat(classOf(self), "%s%s", getStringText(self), getStringText(other), NULL);
}

static StringRef String_copyDescription(const void *restrict const _self) {
	const struct String *self = _self;
	return new(String, getStringText(self), NULL);
}

const void * String = NULL;
const void * StringClass = NULL;

static const char * String_getStringText (const void *const _self) {
	const struct String *self = _self;
	return self->text;
}

static size_t String_getStringLength (const void *const _self) {
	const struct String *self = _self;
	return self->length;
}

static SComparisonResult String_compare (const void *const self, const void *const other) {
	return compareWithOptions(self, other, SStringComparingOptionLiteralSearch);
}

static SComparisonResult String_compareWithOptions (const void *const _self, const void *const _other, SStringComparingOptions options) {
	const struct String *self = _self;
	const struct String *other = _other;
	SComparisonResult result = SSame;
	if (options == SStringComparingOptionLiteralSearch )
		result = strcmp(getStringText(self), getStringText(other));
	else if (options == SStringComparingOptionCaseInsensitiveSearch )
		result =  strcasecmp(getStringText(self), getStringText(other));
	else if (options == SStringComparingOptionLocalizedCompare )
		result = strcoll(getStringText(self), getStringText(other));
	
	if (result<0)
		return SAscending;
	else if	(result>0)
		return SDescending;
	else
		return result;
}

static int String_characterAtIndex(const void * const _self, void *const character, unsigned long index) {
	const struct String *self = _self;
	int result = 0;
	if ( index < getStringLength(self)  && character != NULL )
		*(char *)character = getStringText(self)[index];
	else {
		result = -1;
		errno = EINVAL;
	}
	return result;
}

static int String_getCharactersInRange(const void * const _self, void *const restrict buffer, CORange range) {
	const struct String *self = _self;
	int result = 0;
	uint64_t maxRange = COMaxRange(range);
	uint64_t length = getStringLength(self);
	const char *text = getStringText(self);
	if ( maxRange < length && buffer != NULL ) {
		text = text + range.location;
		memmove(buffer, text, range.length);
//		((char *)buffer)[range.length] = '\0';
	}
	else {
		result = -1;
		errno = EINVAL;
	}
	return result;
}


/* API */

inline uint64_t COMaxRange(CORange range) {
	return (range.location + range.length);
}

inline CORange COMakeRange(uint64_t location, uint64_t length) {
    CORange range;
    range.location = location;
    range.length = length;
    return range;
}

inline int COLocationInRange(uint64_t location, CORange range) {
    return (location - range.location < range.length);
}

inline int COEqualRanges(CORange range1, CORange range2) {
    return (range1.location == range2.location
			&& range1.length == range2.length);
}

static inline void __ltrim(const char *restrict s, CORange *restrict ioRange) {
	if (ioRange->length==0) return;
	
	size_t i=0;
	while(i < ioRange->length && isspace( s[i++] ) );
	ioRange->location += --i;
}

static inline void __rtrim(const char *restrict s, CORange *restrict ioRange) {
	if (ioRange->length == 0) return;
	
	ssize_t i = ioRange->length;
	while(i > ioRange->location && isspace(s[--i]) );
	ioRange->length -= ++i;
}

static inline void __trim(const char *restrict s, CORange *restrict ioRange) { __ltrim(s, ioRange), __rtrim(s, ioRange); }

static StringRef String_copyStringByTrimmingSpaces(const void *const self) {
	CORange range = COMakeRange(0, getStringLength(self));
	const char *text = getStringText(self);
	if ( text == NULL ) return NULL;
	
	__trim( text, &range);
	
	char *result = strndup(text + range.location, range.length);
	if ( result == NULL ) return NULL;
	
	StringRef copy = new(String, result, NULL);
	free(result);
	
	return copy;
}


void initString() {
	if ( ! StringClass )
		StringClass = new(Class, "StringClass", Class, sizeof(struct StringClass), constructor, StringClass_constructor, NULL);
	if ( ! String )
		String = new(StringClass, "String", Object, sizeof(struct String),
					 constructor, String_constructor,
					 destructor, String_destructor,
					 copy, String_copy,
					 equals, String_equals,
					 hash, String_hash,
					 copyDescription, String_copyDescription,
					 
					 /* new */
					 getStringText, String_getStringText,
					 getStringLength, String_getStringLength,
					 copyStringByAppendingString, String_copyStringByAppendingString,
					 newStringWithFormat, String_newStringWithFormat,
					 compareWithOptions, String_compareWithOptions,
					 compare, String_compare,
					 characterAtIndex, String_characterAtIndex,
					 getCharactersInRange, String_getCharactersInRange,
					 
					 
					 copyStringByTrimmingSpaces, String_copyStringByTrimmingSpaces,
					 NULL);
}

void deallocString() {
//	free((void *)String);
//	free((void *)StringClass);
	release((void *)String);
	release((void *)StringClass);
	String = NULL;
	StringClass = NULL;
}

const char *getStringText(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct StringClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->getStringText,ENOTSUP,NULL);
	return class->getStringText(self);
}

uint64_t getStringLength(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,0);
	const struct StringClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,0);
	COAssertNoNullOrReturn(class->getStringLength,ENOTSUP,0);
	return class->getStringLength(self);
}

StringRef copyStringByAppendingString(const void *restrict const self, const void *restrict const other) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	COAssertNoNullOrReturn(other,EINVAL,NULL);
	const struct StringClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->copyStringByAppendingString,ENOTSUP,NULL);
	return class->copyStringByAppendingString(self, other);
}

StringRef newStringWithFormat(const void *const _class, const void *format, ...) {
	COAssertNoNullOrReturn(_class,EINVAL,NULL);
	COAssertNoNullOrReturn(format,EINVAL,NULL);
	
	const struct StringClass *const class = _class;
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->newStringWithFormat,ENOTSUP,NULL);
	
	va_list ap;
	va_start(ap, format);
	StringRef newString = class->newStringWithFormat(class, format, &ap);
	va_end(ap);
	
	return newString;
}

SComparisonResult compare(const void *const self, const void *const other) {
	COAssertNoNullOrReturn(self,EINVAL,-2);
	COAssertNoNullOrReturn(other,EINVAL,-2);
	const struct StringClass *const class = classOf(self);
	const struct StringClass *const classOther = classOf(other);
	COAssertNoNullOrReturn(class,EINVAL,-2);
	COAssertNoNullOrReturn(classOther,EINVAL,-2);
	COAssertNoNullOrReturn(class->compare,ENOTSUP,-2);
	return class->compare(self, other);
}

SComparisonResult compareWithOptions(const void *const self, const void *const other, SStringComparingOptions options) {
	COAssertNoNullOrReturn(self,EINVAL,-2);
	COAssertNoNullOrReturn(other,EINVAL,-2);
	const struct StringClass *const class = classOf(self);
	const struct StringClass *const classOther = classOf(other);
	COAssertNoNullOrReturn(class,EINVAL,-2);
	COAssertNoNullOrReturn(classOther,EINVAL,-2);
	COAssertNoNullOrReturn(class->compareWithOptions,ENOTSUP,-2);
	return class->compareWithOptions(self, other, options);
}


int characterAtIndex(const void * const self, void *const character, uint64_t index) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	const struct StringClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	COAssertNoNullOrReturn(class->characterAtIndex,EINVAL,-1);
	return class->characterAtIndex(self, character, index);
}



int getCharactersInRange(const void * const self, void *const restrict buffer, CORange range) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	COAssertNoNullOrReturn(buffer,EINVAL,-1);
	const struct StringClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	COAssertNoNullOrReturn(class->getCharactersInRange,ENOTSUP,-1);
	return class->getCharactersInRange(self, buffer, range);
}

StringRef copyStringByTrimmingSpaces(const void *const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct StringClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->copyStringByTrimmingSpaces,ENOTSUP,NULL);
	return class->copyStringByTrimmingSpaces(self);
}


