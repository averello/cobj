//
//  Thread.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Thread_h
#define CObjects_Thread_h

#include <pthread.h>

CO_DECLARE_CLASS(Thread)

void startThread(const void *const self);
void joinThread(const void *const self, void **exit);
pthread_t * getPthread(const void *const self);

#endif
