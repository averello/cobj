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

CO_BEGIN_CLASS_TYPE_DECL(Thread,Object)
	voidf threadFunction;
	void * threadArgument;
	pthread_t thread;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(ThreadClass,Classs)
	void ( *startThread ) (const void *const self);
	void ( * joinThread ) (const void *const self, void **exit);
	pthread_t * ( * getPthread ) (const void *const self);
CO_END_CLASS_DECL


#endif
