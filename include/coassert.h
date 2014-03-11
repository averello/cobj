//
//  coassert.h
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_coassert_h
#define CObjects_coassert_h

#include <assert.h>

#if !defined(COASSERT_BLOCK)
#define COAssertDesc(condition, description, ...) assert(condition && description)
#define COAssert(condition) assert((condition))
#define COAssertNoNull(object,err) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
	} \
}

#define COAssertNoNullOrReturn(object,err,ret) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
		return (ret); \
	} \
}

#define COAssertNoNullOrReturnClean(object,err,clean,ret) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
		(clean); \
		return (ret); \
	} \
}


#define COAssertNoNullOrBailOut(object,err) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
		return; \
	} \
}

#define COAssertNoNullOrBailOutClean(object,err,clean) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
		(clean); \
		return; \
	} \
}

#define COAssertNoNullOrReturn2(object,clean,err) \
{ \
	COAssert((object) != NULL); \
	if ((object) == NULL) { \
		errno = (err); \
		(clean); \
		return -(err); \
	} \
}
#else
#define COAssertDesc(condition, description, ...)
#define assert(e)
#define COAssert(condition)

#endif /* !defined(COASSERT_BLOCK) */

#endif /* CObjects_coassert_h */
