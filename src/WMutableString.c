//
//  WMutableString.c
//  CObjects
//
//  Created by George Boumis on 30/11/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>

#include <locale.h>
#include <errno.h>

#include <cobj.h>
#include <WMutableString.r>
#include <WString.r>

extern int errno;

static int __grow(struct WMutableString *const self, UInteger minCapacity) {
	// overflow-conscious code
	// in this function minCapacity is always greater than self->capacity
	struct String *stringSelf = (struct String *)self;
	UInteger newCapacity = 0;
	/* if doubling the capacity is impossible due to overflow */
	if (UINT64_MAX - self->capacity < self->capacity) {
		/* then use the max value */
		newCapacity = UINT64_MAX;
	}
	else {
		newCapacity = self->capacity * 2;
	}

	const void *newText = realloc((void *)stringSelf->text, newCapacity * sizeof(char) );
	if ( newText == NULL ) return -1;
	self->capacity = newCapacity;
	stringSelf->text = newText;
	return 0;
}

inline static int __ensureCapacity(struct WMutableString *const self, UInteger minCapacity) {
	if ( minCapacity > self->capacity )
		return __grow(self, minCapacity);
	return 0;
}


inline static char *__wideToCharConverter(const wchar_t *restrict wideContent) {
	if ( wideContent == NULL ) return errno = EINVAL, (char *)NULL;
	
	size_t charContentSize = wcstombs(NULL, wideContent, 0);
	if ( charContentSize == (size_t)-1 ) return errno = 0, NULL;
	
	char *restrict charContent = calloc(charContentSize+1, sizeof(char));
	if ( charContent == NULL ) return errno = ENOMEM, (char *)NULL;
	
	charContentSize = wcstombs(charContent, wideContent, charContentSize);
	if ( charContentSize == (size_t)-1 ) return free(charContent), errno = 0, (char *)NULL;
	
	return charContent;
}

inline static wchar_t *__charToWideConverter(const char *restrict charContent) {
	if ( charContent == NULL ) return errno = EINVAL, (wchar_t *)NULL;
	
	size_t wideContentSize = mbstowcs(NULL, charContent, 0);
	if ( wideContentSize == (size_t)-1 ) return errno = 0,  (wchar_t  *)NULL;
	
	wchar_t *wideContent = calloc(wideContentSize + 1, sizeof(wchar_t));
	if ( wideContent == NULL ) return errno = 0,  (wchar_t *)NULL;
	
	wideContentSize =mbstowcs(wideContent, charContent, wideContentSize);
	if ( wideContentSize == (size_t)-1 ) return free(wideContent), errno = 0,  (wchar_t *)NULL;
	
	return wideContent;
}


static void * WMutableString_constructor (void * _self, va_list * app) {
	struct WMutableString *self = super_constructor(String, _self, app);
	struct String *super = (struct String *)self;
	
	if (self == NULL) return NULL;
	
	const wchar_t * text = va_arg(* app, const wchar_t *);
	if (NULL==text) {
		super->length = 0;
		self->capacity = 0;
		return self;
	}
	super->text = (wchar_t *)wcsdup(text);
	assert(super->text != NULL);
	if (super->text == NULL) return free(self), NULL;
	super->length = wcslen(super->text);
	UInteger charContentSize = wcstombs(NULL, text, 0);
	self->capacity = (UInteger)charContentSize;
	return self;
}

static void * WMutableStringClass_constructor (void * _self, va_list *app) {
	struct WMutableStringClass * self = super_constructor(WMutableStringClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
	}
	va_end(ap);
	return self;
}

static void * WMutableString_copy (const void *const _self) {
	const struct String *self = _self;
	WStringRef _copy = new(WMutableString, (wchar_t *)self->text, NULL);
	struct String *copy = _copy;
	copy->_hash = self->_hash;
	return _copy;
}

static bool WMutableString_equals (const void * const _self, const void *const _other) {
	const struct String *self = _self;
	const struct String *other = _other;
	const struct Classs *const _super = superclass(superclass(classOf(_self)));
	
	
	int result = _super->equals(_self, _other);
	if ( ! result && (getStringLength(self) == getStringLength(other)) && self->text && other->text)
		result = (wcscmp(self->text, other->text) == 0);
	return result;
}

static UInteger WMutableString_hash(const void *const _self) {
	const struct String *self = _self;
	if (self->_hash == 0) {
		UInteger hash = 0;
		const wchar_t *restrict text = getWText(self);
		for(UInteger i = 0; i < self->length; text++, i++)
			hash = (UInteger)(*text) + (hash << 6) + (hash << 16) - hash;
		((struct String *)self)->_hash = hash;
	}
	return self->_hash;
}

static WMutableStringRef WMutableString_newStringWithFormat(const void *const _class, const wchar_t *const format, va_list *ap) {
	WMutableStringRef newString = NULL;
	wchar_t buffer[BUFSIZ];
	wmemset(buffer, L'\0', BUFSIZ);
	va_list copy;
	va_copy(copy, *ap);
	
	int totalWritten = vswprintf(buffer, BUFSIZ, format, *ap);
	if (totalWritten>= BUFSIZ) {
		wchar_t *newBuffer = calloc((UInteger)totalWritten+1, sizeof(wchar_t));
		assert(newBuffer != NULL);
		vswprintf(newBuffer, (UInteger)totalWritten+1, format, copy);
		newString = new(_class, newBuffer, NULL);
		free(newBuffer);
	}
	else
		newString = new(_class, buffer, NULL);
	va_end(copy);
	return newString;
}

static WMutableStringRef WMutableString_copyStringByAppendingString(const void *restrict const _self, const void *restrict const _other) {
	const struct WString *self = _self;
	const struct WString *other = _other;
	UInteger length = getStringLength(self);
	UInteger otherLength = getStringLength(other);
	UInteger newLength = length + otherLength + sizeof(wchar_t);
	
	wchar_t *buffer = calloc(newLength, sizeof(wchar_t));
	assert(buffer != NULL);
	
	wcsncpy(buffer, getWText(self), length+1);
	buffer[length] = L'\0';
	wcsncat(buffer, (wchar_t *)getWText(other), otherLength);
	WStringRef newString = new(WString, (wchar_t *)buffer, NULL);
	free(buffer);
	return newString;
}

static WMutableStringRef WMutableString_copyDescription(const void *restrict const _self) {
	const struct WString *self = _self;
	char *multibyte = __wideToCharConverter(getWText(self));
	StringRef description  = new(String, multibyte, NULL);
	free(multibyte);
	return description;
}

static SComparisonResult WMutableString_compare (const void *const self, const void *const other) {
	return compareWithOptions(self, other, SStringComparingOptionLiteralSearch);
}

static SComparisonResult WMutableString_compareWithOptions (const void *const _self, const void *const _other, SStringComparingOptions options) {
	const struct WString *self = _self;
	const struct WString *other = _other;
	SComparisonResult result = SSame;
	if (options == SStringComparingOptionLiteralSearch )
		result = wcscmp(getWText(self), getWText(other));
	else if (options == SStringComparingOptionCaseInsensitiveSearch )
		result =  wcscasecmp(getWText(self), getWText(other));
	else if (options == SStringComparingOptionLocalizedCompare )
		result = wcscoll(getWText(self), getWText(other));
	
	if (result<0)
		return SAscending;
	else if	(result>0)
		return SDescending;
	else
		return result;
}

static int WMutableString_characterAtIndex(const void * const _self, void *const character, UInteger index) {
	const struct String *self = _self;
	const wchar_t *text = getWText(self);
	int result = 0;
	if ( index < getStringLength(self)  && character != NULL ) {
		*(wchar_t *)character = text[index];
	}
	else {
		result = -1;
		errno = EINVAL;
	}
	return result;
}

static int WMutableString_getCharactersInRange(const void * const _self, void *const restrict buffer, Range range) {
	const struct String *self = _self;
	int result = 0;
	UInteger maxRange = MaxRange(range);
	UInteger length = getStringLength(self);
	const wchar_t *text = getWText(self);
	if ( maxRange < length && buffer != NULL ) {
		text = text + range.location;
		wmemmove(buffer, text, range.length);
		((wchar_t *)buffer)[range.length] = L'\0';
	}
	else {
		result = -1;
		errno = EINVAL;
	}
	return result;
}

/* Mutable */

static void WMutableString_appendString(void *const _self, const void *const _other) {
	struct WMutableString *self = _self;
	struct String *stringSelf = _self;
	
	UInteger selfLength = getStringLength(self);
	UInteger otherLength = getStringLength(_other);
	
	if (isSubclassOf(_other, WString) || isSubclassOf(_other, WMutableString)) {
		const wchar_t *otherText = getWText(_other);
		
		if ( __ensureCapacity(self, selfLength + otherLength + sizeof(wchar_t)) == -1 ) { errno = ENOMEM; return; };
		
		wchar_t *selfText = (wchar_t *)getStringText(self);
		wcsncat(selfText, otherText, otherLength);
		stringSelf->length += otherLength;
	}
	else {
		const char *otherText = getStringText(_other);
		UInteger size = mbstowcs(NULL, otherText, 0);
		if ( __ensureCapacity(self, selfLength + size + sizeof(wchar_t)) == -1 ) { errno = ENOMEM; return; };
		wchar_t *selfText = (wchar_t *)getStringText(self);
		wchar_t *otherT = __charToWideConverter(otherText);
		wcsncat(selfText, otherT, otherLength);
		stringSelf->length += wcslen(otherT);
		free(otherT);
	}
}

const void * WMutableString = NULL;
const void * WMutableStringClass = NULL;

void initWMutableString() {
	initMutableString();
	if ( !WMutableStringClass )
		WMutableStringClass = new(MutableStringClass, "WMutableStringClass", MutableStringClass, sizeof(struct WMutableStringClass), constructor, WMutableStringClass_constructor, NULL);
	if ( !WMutableString )
		WMutableString = new(WMutableStringClass, "WMutableString", MutableString, sizeof(struct WMutableString),
							 constructor, WMutableString_constructor,

							 copy, WMutableString_copy,
							 equals, WMutableString_equals,
							 hash, WMutableString_hash,

							 appendString, WMutableString_appendString,
							 copyStringByAppendingString, WMutableString_copyStringByAppendingString,
							 newStringWithFormat, WMutableString_newStringWithFormat,
							 copyDescription, WMutableString_copyDescription,
							 compare, WMutableString_compare,
							 compareWithOptions, WMutableString_compareWithOptions,
							 characterAtIndex, WMutableString_characterAtIndex,
							 getCharactersInRange, WMutableString_getCharactersInRange,
							 NULL);
}

void deallocWMutableString() {
//	free((void *)WMutableString);
//	free((void *)WMutableStringClass);
	release((void *)WMutableString);
	release((void *)WMutableStringClass);
	WMutableString = NULL;
	WMutableStringClass = NULL;
	deallocMutableString();
}

