//
//  coassert.h
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_coassert_h
#define CObjects_coassert_h

#define COASSERT_BLOCK NDEBUG

#if !defined(COASSERT_BLOCK)
#define COAssert(condition, description, ...) assert(condition && description)
//\
//	do { \
//		if ( !(condition) ) { \
//			\
//		} \
//	} while(0)
#else
#define COAssert(condition, description, ...)
#endif
