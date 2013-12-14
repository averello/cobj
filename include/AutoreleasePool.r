//
//  AutoreleasePool.r
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_AutoreleasePool_r
#define CObjects_AutoreleasePool_r

#include <Object.h>
#include <Object.r>
#include <AutoreleasePool.h>
#include <MutableArray.h>
#include <sys/queue.h>


struct AutoreleasePoolListItem {
	void * object;
	SLIST_ENTRY(AutoreleasePoolListItem) entry;
};

struct AutoreleasePool {
	const struct Object isa;
	SLIST_HEAD(AutoreleasePoolListHead, AutoreleasePoolListItem) list;
};



struct AutoreleasePoolClass {
	const struct Classs isa;
	void (* addAutoreleaseObject)(const void *self, const void *object);
};


#endif
