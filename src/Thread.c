//
//  Thread.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <stdarg.h>
#include <pthread.h>

#include <cobj.h>
#include <Thread.r>

const void * Thread = NULL;
const void * ThreadClass = NULL;

static void * Thread_constructor (void * _self, va_list * app) {
	struct Thread *self = super_constructor(Thread, _self, app);
	{
		voidf threadFunction = va_arg(*app, voidf);
		if (threadFunction == NULL) return free(self), NULL;
		self->threadFunction = threadFunction;
	}
	
	void *threadArgument = va_arg(*app, void *);
	self->threadArgument = threadArgument;

	return self;
}

static void * Thread_destructor (void * _self) {
	struct Thread *self = super_destructor(Thread, _self);
	return self;
}

static void * ThreadClass_constructor (void * _self, va_list *app) {
	struct ThreadClass * self = super_constructor(ThreadClass, _self, app);
	typedef void (*voidf) ();
	voidf selector;
	va_list ap;
	va_copy(ap, *app);
	while ( (selector = va_arg(ap, voidf)) ) {
		voidf method = va_arg(ap, voidf);
		if (selector == (voidf) startThread )
			* (voidf *) & self->startThread = method;
		else if (selector == (voidf) joinThread )
			* (voidf *) & self->joinThread = method;
		else if (selector == (voidf) getPthread )
			* (voidf *) & self->getPthread = method;
	}
	va_end(ap);
	return self;
}

static void * Thread_copy (const void * const _self) {
	return NULL;
}

static int Thread_equals (const void * const _self, const void *const _other) {
	const struct Thread *self = _self;
	const struct Thread *other = _other;
	const struct Class *const _super = (const struct Class *const )super(_self);
	
	return (
			_super->equals(_self, _other)
			||
			(self->thread == other->thread)
			);
}

static void Thread_startThread(const void *const _self) {
	struct Thread *self = (struct Thread *)_self;
	pthread_create(&(self->thread), NULL, self->threadFunction, self->threadArgument);
}

static void Thread_joinThread(const void *const _self, void **exit) {
	struct Thread *self = (struct Thread *)_self;
	pthread_join(self->thread, exit);
}

static pthread_t *Thread_getPthread(const void *const _self) {
	struct Thread *self = (struct Thread *)_self;
	return &(self->thread);
}

void initThread() {
	if ( ! ThreadClass )
		ThreadClass = new(Class, "ThreadClass", Class, sizeof(struct ThreadClass),
						  constructor, ThreadClass_constructor, NULL);
	if ( ! Thread )
		Thread = new(ThreadClass, "Thread", Object, sizeof(struct Thread),
					 constructor, Thread_constructor,
					 destructor, Thread_destructor,
					 
					 /* Oveerrides */
					 copy, Thread_copy,
					 equals, Thread_equals,
					 
					 /* new */
					 startThread, Thread_startThread,
					 joinThread, Thread_joinThread,
					 getPthread, Thread_getPthread,
					 NULL);
}

void deallocThread() {
	free((void *)Thread), Thread = NULL;
	free((void *)ThreadClass), ThreadClass = NULL;
}


/* API */

void startThread(const void *const self) {
	assert( self != NULL );
	const struct ThreadClass *const class = classOf(self);
	assert( class != NULL && class->startThread );
	class->startThread(self);
}

void joinThread(const void *const self, void **exit) {
	assert( self != NULL );
	const struct ThreadClass *const class = classOf(self);
	assert( class != NULL && class->joinThread );
	class->joinThread(self, exit);
}

pthread_t * getPthread(const void *const self) {
	assert( self != NULL );
	const struct ThreadClass *const class = classOf(self);
	assert( class != NULL && class->getPthread );
	return class->getPthread(self);
}

