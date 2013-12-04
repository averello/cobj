//
//  WString.c
//  CObjects
//
//  Created by George Boumis on 13/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <locale.h>
#include <errno.h>

#include <cobj.h>
#include <WString.r>
#include <StringObject.r>

extern int errno;

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


static void * WString_constructor (void * _self, va_list * app) {
	struct WString *self = super_constructor(String, _self, app);
	struct String *super = (struct String *)self;
	
	const wchar_t * text = va_arg(* app, const wchar_t *);
	if (NULL==text) {
		super->length = 0;
		return self;
	}
	super->text = (wchar_t *)wcsdup(text);
	assert(super->text != NULL);
	if ( super->text == NULL ) return free(self), NULL;
	super->length = wcslen(super->text);
	super->_hash = 0;
	return self;
}

static void * WStringClass_constructor (void * _self, va_list *app) {
	struct WStringClass * self = super_constructor(WStringClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
//		voidf method = va_arg(ap, voidf);
//		if (selector == (voidf) getStringLength)
//			* (voidf *) & self->getStringLength = method;
//		else if (selector == (voidf) compareWithOptions )
//			* (voidf *) & self->compareWithOptions = method;
	}
	va_end(ap);
	return self;
}

static void * WString_copy (const void *const _self) {
	const struct String *self = _self;
	WStringRef _copy = new(WString, (wchar_t *)self->text, NULL);
	struct String *copy = _copy;
	copy->_hash = self->_hash;
	return _copy;
}

static int WString_equals (const void * const _self, const void *const _other) {
	const struct String *self = _self;
	const struct String *other = _other;
	const struct Classs *const _super = superclass(superclass(classOf(_self)));
	
	
	int result = _super->equals(_self, _other);
	if ( ! result && (getStringLength(self) == getStringLength(other)) && self->text && other->text)
		result = (wcscmp(self->text, other->text) == 0);
	return result;
}

static int WString_hash(const void *const _self) {
	const struct String *self = _self;
	if (self->_hash == 0) {
		int hash = 0;
		const wchar_t *restrict text = getWText(self);
		for(long i = 0; i < self->length; text++, i++)
			hash = (*text) + (hash << 6) + (hash << 16) - hash;
		((struct String *)self)->_hash = hash;
	}
	return self->_hash;
}

static WStringRef WString_newStringWithFormat(const void *const _class, const wchar_t *const format, va_list *ap) {
	WStringRef newString = NULL;
	wchar_t buffer[BUFSIZ];
	wmemset(buffer, L'\0', BUFSIZ);
	va_list copy;
	va_copy(copy, *ap);
	
	int totalWritten = vswprintf(buffer, BUFSIZ, format, *ap);
	if (totalWritten>= BUFSIZ) {
		wchar_t *newBuffer = calloc(totalWritten+1, sizeof(wchar_t));
		assert(newBuffer != NULL);
		vswprintf(newBuffer, totalWritten+1, format, copy);
		newString = new(_class, newBuffer, NULL);
		free(newBuffer);
	}
	else
		newString = new(_class, buffer, NULL);
	va_end(copy);
	return newString;
}

static WStringRef WString_copyStringByAppendingString(const void *restrict const _self, const void *restrict const _other) {
	const struct WString *self = _self;
	const struct WString *other = _other;
	size_t length = getStringLength(self);
	size_t otherLength = getStringLength(other);
	size_t newLength = length + otherLength + sizeof(wchar_t);
	
	wchar_t *buffer = calloc(newLength, sizeof(wchar_t));
	assert(buffer != NULL);
	
	wcsncpy(buffer, getWText(self), length+1);
	buffer[length] = L'\0';
	wcsncat(buffer, (wchar_t *)getWText(other), otherLength);
	WStringRef newString = new(WString, (wchar_t *)buffer, NULL);
	free(buffer);
	return newString;
}

static WStringRef WString_copyDescription(const void *restrict const _self) {
	const struct WString *self = _self;
	char *multibyte = __wideToCharConverter(getWText(self));
	StringRef description  = new(String, multibyte, NULL);
	free(multibyte);
	return description;
}

static SComparisonResult WString_compare (const void *const self, const void *const other) {
	return compareWithOptions(self, other, SStringComparingOptionLiteralSearch);
}

static SComparisonResult WString_compareWithOptions (const void *const _self, const void *const _other, SStringComparingOptions options) {
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

static int WString_characterAtIndex(const void * const _self, void *const character, unsigned long index) {
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

static int WString_getCharactersInRange(const void * const _self, void *const restrict buffer, SRange range) {
	const struct String *self = _self;
	int result = 0;
	unsigned long maxRange = SMaxRange(range);
	size_t length = getStringLength(self);
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


const void * WString = NULL;
const void * WStringClass = NULL;

void initWString() {
	setlocale(LC_CTYPE, "UTF-8");
	initString();
	if ( ! WStringClass )
		WStringClass = new(StringClass, "WStringClass", StringClass, sizeof(struct WStringClass), constructor, WStringClass_constructor, NULL);
	if ( ! WString )
		WString = new(WStringClass, "WString", String, sizeof(struct WString),
					  constructor, WString_constructor,
					  copy, WString_copy,
					  equals, WString_equals,
					  copyStringByAppendingString, WString_copyStringByAppendingString,
					  newStringWithFormat, WString_newStringWithFormat,
					  copyDescription, WString_copyDescription,
					  compare, WString_compare,
					  compareWithOptions, WString_compareWithOptions,
					  characterAtIndex, WString_characterAtIndex,
					  getCharactersInRange, WString_getCharactersInRange,
					  hash, WString_hash,
					  NULL);
}

void deallocWString() {
	release((void *)WString);
	release((void *)WStringClass);
//	free((void *)WString);
//	free((void *)WStringClass);
	WString = NULL;
	WStringClass = NULL;
	deallocString();
}

const wchar_t *getWText(const void *const self) {
	assert(self);
	const struct StringClass *const class = classOf(self);
	assert(class != NULL && class->getStringText != NULL);
	return (const wchar_t *)class->getStringText(self);
}
