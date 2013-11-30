//
//  Thread.r
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Thread_r
#define CObjects_Thread_r

#include <cobj.h>
#include <Object.r>
#include <pthread.h>

typedef void* (*voidf) (void *);

struct Thread {
	const struct Object isa;

	voidf threadFunction;
	void * threadArgument;
	pthread_t thread;
};

struct ThreadClass {
	const struct Class isa;
	void ( *startThread ) (const void *const self);
	void ( * joinThread ) (const void *const self, void **exit);
	pthread_t * ( * getPthread ) (const void *const self);
};


#endif
