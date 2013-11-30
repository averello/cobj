/*!
 *  @file ConcurrentMutableArray.r
 *  @brief CObjects
 *
 *  Created by @author George Boumis 
 *  @date 27/4/13.
 *  @copyright   Copyright (c) 2013 George Boumis. All rights reserved.
 */

#ifndef CObjects_ConcurrentMutableArray_r
#define CObjects_ConcurrentMutableArray_r

#include <cobj.h>
#include <Object.r>
#include <MutableArray.r>
#include <pthread.h>

struct ConcurrentMutableArray {
	const struct MutableArray isa;
	pthread_mutex_t protector;
	pthread_cond_t synchronization;
};

struct ConcurrentMutableArrayClass {
	const struct MutableArrayClass isa;
};


#endif
