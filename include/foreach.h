//
//  foreach.h
//  CObjects
//
//  Created by George Boumis on 9/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_foreach_h
#define CObjects_foreach_h

#include <Object.h>
#include <assert.h>
#include <string.h>

typedef struct {
    UInteger state;
    void **itemsPointer;
    UInteger *mutationsPointer;
    UInteger extra[5];
} FastEnumerationState;

UInteger enumerateWithState(const void *const collection, FastEnumerationState *const state, void *iobuffer[], UInteger length);

#ifndef foreach_start
#define foreach_start(type,variable,collection) \
{\
	FastEnumerationState ___state;\
	memset(&___state, 0, sizeof(FastEnumerationState));\
	ObjectRef ___buffer[16];\
	memset(&___buffer, 0, sizeof(___buffer));\
	int ___firstLoop = 1;\
	UInteger ___mutationsPointerValue;\
	UInteger ___count;\
	while ((___count = enumerateWithState((collection), &___state, ___buffer, 16))) {\
		if (!___firstLoop && ___mutationsPointerValue != *___state.mutationsPointer) {\
			assert(0 && "Object mutated while iterating");\
		}\
		___firstLoop = 0;\
		___mutationsPointerValue = *___state.mutationsPointer;\
		type variable;\
		for (UInteger ___index=0; ___index<___count; ___index++) {\
			variable = ___state.itemsPointer[___index];

#define foreach_end() }}}

#endif

#endif /* CObjects_foreach_h */
