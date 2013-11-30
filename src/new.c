//
//  new.c
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <new.h>
#include <Object.h>
#include <Object.r>
#include <stdarg.h>
#include <sys/types.h>
#include <errno.h>

#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

/* Creates any Object */
void * new (const void *const restrict _class, ...) {
	if ( _class == NULL ) return errno = EINVAL, NULL;
	assert( _class != NULL );
	
	/* Get the type */
	const struct Class *const class = _class;
	struct Object * object;
	va_list ap;
	
	if ( class->size <= 0 ) return  NULL;
	
	object = calloc(1, class->size);
	assert(object != NULL);
	if ( object == NULL ) return errno = ENOMEM, NULL;
	object->class = class;
	
	va_start(ap, _class);
	object = constructor(object, &ap);
	va_end(ap);
	
	return object;
}

void delete (void * self) {
	if ( self != NULL )
		destructor(self);
	else
		errno = EINVAL;
}
