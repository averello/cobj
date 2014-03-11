//
//  AutoreleasePool.c
//  CObjects
//
//  Created by George Boumis on 13/12/13.
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

#include <AutoreleasePool.h>
#include <AutoreleasePool.r>
#include <new.h>
#include <Object.h>
#include <Object.r>
#include <MutableDictionary.h>
#include <foreach.h>
#include <memory_management/memory_management.h>
#undef release
#undef retain


extern int errno;

struct threadAutoreleasePoolsHeadItem {
	AutoreleasePoolRef autoreleasePool;
	TAILQ_ENTRY(threadAutoreleasePoolsHeadItem) entries;
};
__thread TAILQ_HEAD(ThreadAutoreleasePoolsHead, threadAutoreleasePoolsHeadItem) ThreadAutoreleasePools;

static void * AutoreleasePool_constructor(void * _self, va_list * app) {
	struct AutoreleasePool *self = super_constructor(AutoreleasePool, _self, app);
	if (ThreadAutoreleasePools.tqh_last == NULL && ThreadAutoreleasePools.tqh_first == NULL) {
		TAILQ_INIT(&ThreadAutoreleasePools);
	}
	struct threadAutoreleasePoolsHeadItem *item = MEMORY_MANAGEMENT_ALLOC(sizeof(struct threadAutoreleasePoolsHeadItem));
	item->autoreleasePool = self;
	TAILQ_INSERT_TAIL(&ThreadAutoreleasePools, item, entries);
	
	SLIST_INIT(&self->list);
	return self;
}

static void * AutoreleasePoolClass_constructor(void * _self, va_list * app) {
	struct AutoreleasePoolClass *self = super_constructor(AutoreleasePoolClass, _self, app);
	
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) addAutoreleaseObject)
			* (voidf *) & self->addAutoreleaseObject = method;
	}
	va_end(ap);
	
	return self;
}

static void * AutoreleasePool_destructor(void * _self, va_list * app) {
	struct AutoreleasePool *self = _self;
	struct AutoreleasePoolListItem *item = NULL;
	while ( (item = SLIST_FIRST(&self->list))) {
		SLIST_REMOVE_HEAD(&self->list, entry);
		release((void *)item->object);
		MEMORY_MANAGEMENT_RELEASE(item);
	}
	
	{
		/* Pop other autorelease pools */
		struct threadAutoreleasePoolsHeadItem *item = NULL;
		while( (item = TAILQ_LAST(&ThreadAutoreleasePools, ThreadAutoreleasePoolsHead)) ) {
			if (item->autoreleasePool == self) {
				TAILQ_REMOVE(&ThreadAutoreleasePools, item, entries);
				MEMORY_MANAGEMENT_RELEASE(item);
				break;
			}
			else
				release(item->autoreleasePool);
		}
	}
	return super_destructor(AutoreleasePool, _self);
}

static void AutoreleasePool_addAutoreleaseObject(void * _self, void *object) {
	struct AutoreleasePool *self = _self;
	struct AutoreleasePoolListItem *item = MEMORY_MANAGEMENT_ALLOC((sizeof(struct AutoreleasePoolListItem)));
	item->object = object;
	SLIST_INSERT_HEAD(&self->list, item, entry);
}

static void * AutoreleasePool_copy (const void *const _self) {
	const struct AutoreleasePool *self = _self;
	return (void *)self;
}

void * AutoreleasePool_retain (void * const _self) {
	return _self;
}




const void * AutoreleasePool = NULL;

/*! A @ref StringClass type */
const void * AutoreleasePoolClass = NULL;

void initAutoreleasePool() {
	memset(&ThreadAutoreleasePools, 0, sizeof(ThreadAutoreleasePools));
	if ( ! AutoreleasePoolClass )
		AutoreleasePoolClass = new(Class, "AutoreleasePoolClass", Class, sizeof(struct AutoreleasePoolClass),
								   constructor, AutoreleasePoolClass_constructor);
	if ( ! AutoreleasePool )
		AutoreleasePool = new(AutoreleasePoolClass, "AutoreleasePool", Object, sizeof(struct AutoreleasePool),
							  constructor, AutoreleasePool_constructor,
							  destructor, AutoreleasePool_destructor,
							  
							  /* Overrides */
							  copy, AutoreleasePool_copy,
							  retain, AutoreleasePool_retain,
							  
							  /* new */
							  addAutoreleaseObject, AutoreleasePool_addAutoreleaseObject,
							  NULL);
}

void deallocAutoreleasePool() {
	if (AutoreleasePool) release((void *)AutoreleasePool), AutoreleasePool = NULL;
	if (AutoreleasePoolClass) release((void *)AutoreleasePoolClass), AutoreleasePool = NULL;
}






/* API */

void addAutoreleaseObject(const void *self, const void *object) {
	COAssertNoNullOrBailOut(self,EINVAL);
	
	const struct AutoreleasePoolClass *class = classOf(self);
	COAssertNoNullOrBailOut(class,EINVAL);
	COAssertNoNullOrBailOut(class->addAutoreleaseObject,ENOTSUP);
	class->addAutoreleaseObject(self, object);
}


void AutoreleasePoolAddObject(const void *object) {
	COAssertNoNullOrBailOut(object,EINVAL);
	
	if (ThreadAutoreleasePools.tqh_last == NULL && ThreadAutoreleasePools.tqh_first == NULL)
		TAILQ_INIT(&ThreadAutoreleasePools);
	struct threadAutoreleasePoolsHeadItem *item = TAILQ_LAST(&ThreadAutoreleasePools, ThreadAutoreleasePoolsHead);
	if (item == NULL) {
		fprintf(stderr, "Object of class %s autoreleased with no pool, just leaking\n", getClassName(object));
		return;
	}
	else {
		struct AutoreleasePool *pool = item->autoreleasePool;
		addAutoreleaseObject(pool, object);
	}
}

