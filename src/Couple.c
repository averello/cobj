//
//  Couple.c
//  CObjects
//
//  Created by averello on 3/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <cobj.h>
#include <Couple.r>

const void * Couple = NULL;
const void * CoupleClass = NULL;

static void * Couple_constructor (void * _self, va_list * app) {
	struct Couple *self = super_constructor(Couple, _self, app);
	
	ObjectRef key = NULL, value = NULL;
	key = va_arg(*app, void *);
	value = va_arg(*app, void *);
	if ( key == NULL || value == NULL ) return free(self), errno = EINVAL, NULL;
	
	retain(key), retain(value);
	self->key = key;
	self->value = value;
	return self;
}

static void * Couple_destructor (void * _self) {
	struct Couple *self = super_destructor(Couple, _self);
	release(self->key), release(self->value);
	return self;
}

static void * CoupleClass_constructor (void * _self, va_list *app) {
	struct CoupleClass * self = super_constructor(CoupleClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) getKey )
			* (voidf *) & self->getKey = method;
		else if (selector == (voidf) getValue )
			* (voidf *) & self->getValue = method;
		else if (selector == (voidf) setValue )
			* (voidf *) & self->setValue = method;
		else if (selector == (voidf) setKey )
			* (voidf *) & self->setKey = method;
	}
	va_end(ap);
	return self;
}

static void * Couple_copy (const void * const _self) {
	const struct Couple *self = _self;
	return new(Couple, getKey(self), getValue(self), NULL);
}

static bool Couple_equals (const void * const _self, const void *const _other) {
	const struct Couple *const self = _self;
	const struct Couple *const other = _other;
	const struct Classs *const _super = (const struct Classs *const )super(_self);
	
	
	bool result = _super->equals(_self, _other);
	if ( ! result )
		return (
				equals(getKey(self), getKey(other))
				&&
				equals(getValue(self), getValue(other))
				);
	return result;
}

static UInteger Couple_hash (const void *const _self) {
	const struct Object *const self = _self;
	UInteger prime = 31;
	UInteger result = 1;
	result = prime * result + hash(getKey(self));
	result = prime * result + hash(getValue(self));
	return result;
}

static void * Couple_getKey (const void *const _self) {
	const struct Couple *const self = _self;
	return self->key;
}

static void * Couple_getValue (const void *const _self) {
	const struct Couple *const self = _self;
	return self->value;
}

static void Couple_setKey (void *const _self, void * const key) {
	struct Couple *const self = _self;
	release(self->key);
	self->key = retain(key);
}

static void Couple_setValue (void *const _self, void * const value) {
	struct Couple *const self = _self;
	release(self->value);
	self->value = retain(value);
}

static StringRef Couple_copyDescription (void *const _self) {
	struct Couple *const self = _self;
	const struct Class *const _super = (const struct Class *const )super(_self);
	StringRef supercopyDescription = copyDescription(_super);
	
	StringRef keycopyDescription = getKey(self);
	StringRef valuecopyDescription = getValue(self);
	StringRef copyDescription = newStringWithFormat(String, "<%s key:\"%s\" value:\"%s\">", getStringText(supercopyDescription), getStringText(keycopyDescription), getStringText(valuecopyDescription), NULL);
	
	release(supercopyDescription), release(keycopyDescription), release(valuecopyDescription);
	return copyDescription;
}

void initCouple() {
	if ( ! CoupleClass ) {
		CoupleClass = new(Class, "CoupleClass", Class, sizeof(struct CoupleClass),
						  constructor, CoupleClass_constructor, NULL);
	}
	if ( ! Couple ) {
		Couple = new(CoupleClass, "Couple", Object, sizeof(struct Couple),
					 constructor, Couple_constructor,
					 destructor, Couple_destructor,
					 /* Overrides */
					 equals, Couple_equals,
					 copy, Couple_copy,
					 copyDescription, Couple_copyDescription,
					 hash, Couple_hash,
					 
					 /* new */
					 getKey, Couple_getKey,
					 getValue, Couple_getValue,
					 setKey, Couple_setKey,
					 setValue, Couple_setValue,
					 NULL);
	}
}

void deallocCouple() {
	release((void *)Couple), Couple = NULL;
	release((void *)CoupleClass), CoupleClass = NULL;
//	free((void *)Couple), Couple = NULL;
//	free((void *)CoupleClass), CoupleClass = NULL;
}


/* API */

void * getKey(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct CoupleClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->getKey,ENOTSUP,NULL);
	return class->getKey(self);
}

void * getValue(const void * const self) {
	COAssertNoNullOrReturn(self,EINVAL,NULL);
	const struct CoupleClass *class = classOf(self);
	COAssertNoNullOrReturn(class,EINVAL,NULL);
	COAssertNoNullOrReturn(class->getValue,ENOTSUP,NULL);
	return class->getValue(self);
}

void setKey(void * const self, void * const key) {
	COAssertNoNullOrBailOut(self,EINVAL);
	const struct CoupleClass *class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->setKey,ENOTSUP);
	class->setKey(self, key);
}

void setValue(void * const self, void * const value) {
	COAssertNoNullOrBailOut(self,EINVAL);
	const struct CoupleClass *class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->getKey,ENOTSUP);
	class->setValue(self, value);
}




