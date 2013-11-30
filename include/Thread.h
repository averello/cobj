//
//  Thread.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Thread_h
#define CObjects_Thread_h

extern const void * Thread;
extern const void * ThreadClass;
typedef void * ThreadRef;

#include <pthread.h>

void initThread() __attribute__ ((constructor));
void deallocThread() __attribute__ ((destructor));

void startThread(const void *const self);
void joinThread(const void *const self, void **exit);
pthread_t * getPthread(const void *const self);

#endif
