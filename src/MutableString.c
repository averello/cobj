//
//  MutableString.c
//  CObjects
//
//  Created by averello on 6/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <cobj.h>
#include <MutableString.r>

const void * MutableString = NULL;
const void * MutableStringClass = NULL;

static int __grow(struct MutableString *const self, UInteger minCapacity) {
	// overflow-conscious code
	// in this function minCapacity is always greater than self->capacity
	struct String *stringSelf = (struct String *)self;
	UInteger newCapacity = 0;
	/* if doubling the capacity is impossible due to overflow */
	if (UIntegerMax - self->capacity < self->capacity) {
		/* then use the max value */
		newCapacity = UIntegerMax;
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

inline static int __ensureCapacity(struct MutableString *const self, UInteger minCapacity) {
	if ( minCapacity > self->capacity )
		return __grow(self, minCapacity);
	return 0;
}


static void * MutableString_constructor (void * _self, va_list * app) {
	struct MutableString *self = super_constructor(MutableString, _self, app);
	if ( self == NULL ) return NULL;
	self->capacity = getStringLength(self) + 1;
	return self;
}

static void * MutableString_destructor (void * _self) {
	struct MutableString *self = super_destructor(MutableString, _self);
	return self;
}

static void * MutableStringClass_constructor (void * _self, va_list *app) {
	struct MutableStringClass * self = super_constructor(MutableStringClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) appendString )
			* (voidf *) & self->appendString = method;
		
		else if (selector == (voidf) appendFormat )
			* (voidf *) & self->appendFormat = method;
		
		else if (selector == (voidf) setString )
			* (voidf *) & self->setString = method;
		else if (selector == (voidf) setMutableStringLength )
			* (voidf *) & self->setMutableStringLength = method;
		
		else if (selector == (voidf) insertStringAtMutableStringIndex )
			* (voidf *) & self->insertStringAtMutableStringIndex = method;
		else if (selector == (voidf) deleteMutableStringCharactersInRange )
			* (voidf *) & self->deleteMutableStringCharactersInRange = method;
	}
	va_end(ap);
	return self;
}

static void * MutableString_copy (const void * const _self) {
	const struct MutableString *self = _self;
	return new(MutableString, getStringText(self), NULL);
}

static bool MutableString_equals (const void * const _self, const void *const _other) {
	const struct String *self = _self;
	const struct String *other = _other;
	int result = ( _self == _other );
	if ( ! result && (getStringLength(self) == getStringLength(other)) && self->text && other->text)
		result = (strcmp(self->text, other->text) == 0);
	return result;
}

static void MutableString_appendString(void *const _self, const void *const _other) {
	struct MutableString *self = _self;
	struct String *stringSelf = _self;
	const struct MutableString *other = _other;
	const char *otherText = getStringText(other);
	
	UInteger selfLength = getStringLength(self);
	UInteger otherLength = getStringLength(other);
	
	if ( __ensureCapacity(self, selfLength + otherLength + 1) == -1 ) { errno = ENOMEM; return; };
	
	char *selfText = (char *)getStringText(self);
	strncat(selfText, otherText, otherLength);
	stringSelf->length += otherLength;
}

static UInteger MutableString_getStringLength (const void *const _self) {
	const struct String *self = _self;
	return self->length;
}

static void MutableString_appendFormat(void *const self, char *format, va_list *app) {
	struct String *stringSelf = self;
	char *selfText = NULL;
	UInteger selfLength = getStringLength(self);
	
	va_list copy;
	va_copy(copy, *app);
	int totalWritten = vsnprintf(NULL, 0, format, copy);
	char *text = calloc((UInteger)totalWritten+1, sizeof(char));
	if ( text == NULL ) { errno = ENOMEM, va_end(copy); return; }
	if ( __ensureCapacity(self, selfLength + (UInteger)totalWritten + 1) == -1 ) {
		errno = ENOMEM, va_end(copy), free(text); return; 
	};
	selfText = (char *)getStringText(self);
	
	vsnprintf(text, totalWritten+1, format, *app);
	strncat(selfText, text, totalWritten);

	stringSelf->length += (UInteger)totalWritten;
	
	free(text), va_end(copy);
}

static void MutableString_setString(void *const _self, const void *const other) {
	struct MutableString *self = _self;
	struct String *stringSelf = _self;
	
	char *text = strdup(getStringText(other));
	if ( text == NULL ) { return; }
	
	free((char *)getStringText(self)), stringSelf->text = NULL;
	stringSelf->text = text;
	
	UInteger otherLength = getStringLength(other);
	stringSelf->length = otherLength;
	self->capacity = otherLength + 1;
	self->offset = 0;
}

static void MutableString_setMutableStringLength(void *const _self, UInteger capacity) {
	struct MutableString *self = _self;
	struct String *stringSelf = _self;
	
	UInteger selfCapacity = self->capacity;
	if ( selfCapacity < capacity ) {
		if ( __ensureCapacity(self, capacity+1) == -1 ) { errno = ENOMEM; return; };
		memset(((char*)stringSelf->text)+selfCapacity, '\0', capacity+1 - selfCapacity);
	}
	else {
		char *text = (char *)stringSelf->text;
		text = realloc(text, capacity+1);
		if ( text == NULL ) { errno = ENOMEM; return; };
		text[capacity] = '\0';
		stringSelf->text = text;
		stringSelf->length = capacity;
	}
	
	self->capacity = capacity+1;
}

static int MutableString_insertStringAtMutableStringIndex(void *const _self, const void *const other, UInteger index) {
	struct MutableString *self = _self;
	struct String *stringSelf = _self;
	
	UInteger selfLength = getStringLength(self);
	UInteger otherLength = getStringLength(other);
	
	if ( index > selfLength ) return errno = EINVAL, -1;
	if ( index == selfLength ) return appendString(self, other), 0;
	
	if ( __ensureCapacity(self, selfLength + otherLength + 1) == -1 ) return errno = ENOMEM, -1;
	
	char *selfText = (char *)getStringText(self);
	char *otherText = (char *)getStringText(other);
	
	memmove(selfText+(index+otherLength), selfText+index, selfLength-(index-otherLength));
	strncpy(selfText+index, otherText, otherLength);
	
	stringSelf->length += otherLength;
	
	return 0;
}

static int MutableString_deleteMutableStringCharactersInRange(void *const _self, Range range) {
	struct MutableString *self = _self;
	struct String *stringSelf = _self;
	
	if ( MaxRange(range) > stringSelf->length ) return errno = EINVAL, -1;
	
	UInteger selfLength = getStringLength(self);
	char *selfText = (char *)getStringText(self);
	
	memmove(selfText+range.location, selfText+MaxRange(range), selfLength-MaxRange(range)+1);
	stringSelf->length -= range.length;
	
	return -1;
}


void initMutableString() {
	initString();
	
	if ( ! MutableStringClass )
		MutableStringClass = new(StringClass, "MutableStringClass", StringClass, sizeof(struct MutableStringClass),
						 constructor, MutableStringClass_constructor, NULL);
	if ( ! MutableString )
		MutableString = new(MutableStringClass, "MutableString", String, sizeof(struct MutableString),
					constructor, MutableString_constructor,
					destructor, MutableString_destructor,
					
					/* Overrides */
					copy, MutableString_copy,
					equals, MutableString_equals,
					getStringLength, MutableString_getStringLength,
					
					/* new */
					appendString, MutableString_appendString,
					appendFormat, MutableString_appendFormat,
					setString, MutableString_setString,
					setMutableStringLength, MutableString_setMutableStringLength,
					insertStringAtMutableStringIndex, MutableString_insertStringAtMutableStringIndex,
					deleteMutableStringCharactersInRange, MutableString_deleteMutableStringCharactersInRange,
					NULL);
}

void deallocMutableString() {
//	free((void *)MutableString), MutableString = NULL;
//	free((void *)MutableStringClass), MutableStringClass = NULL;
	release((void *)MutableString), MutableString = NULL;
	release((void *)MutableStringClass), MutableStringClass = NULL;
	deallocString();
}


/* API */


void appendString(void *const self, const void *const other) {
	COAssertNoNullOrBailOut(self,EINVAL);
	COAssertNoNullOrBailOut(other,EINVAL);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->appendString,ENOTSUP);
	class->appendString(self, other);
}

void appendFormat(void *const self, char *format, ...) {
	COAssertNoNullOrBailOut(self,EINVAL);
	COAssertNoNullOrBailOut(format,EINVAL);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->appendFormat,ENOTSUP);
	
	va_list ap;
	va_start(ap, format);
	class->appendFormat(self, format, &ap);
	va_end(ap);
}


void setString(void *const self, const void *const other) {
	COAssertNoNullOrBailOut(self,EINVAL);
	COAssertNoNullOrBailOut(other,EINVAL);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->setString,ENOTSUP);
	class->setString(self, other);
}

void setMutableStringLength(void *const self, UInteger capacity) {
	COAssertNoNullOrBailOut(self,EINVAL);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->setMutableStringLength,ENOTSUP);
	class->setMutableStringLength(self, capacity);
}

int insertStringAtMutableStringIndex(void *const self, const void *const other, UInteger index) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	COAssertNoNullOrReturn(other,EINVAL,-1);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	COAssertNoNullOrReturn(class->insertStringAtMutableStringIndex,ENOTSUP,-1);
	return class->insertStringAtMutableStringIndex(self, other, index);
}

int deleteMutableStringCharactersInRange(void *const self, Range range) {
	COAssertNoNullOrReturn(self,EINVAL,-1);
	
	const struct MutableStringClass *const class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,-1);
	COAssertNoNullOrReturn(class->deleteMutableStringCharactersInRange,ENOTSUP,-1);
	return class->deleteMutableStringCharactersInRange(self, range);

}

