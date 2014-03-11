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
    uint64_t state;
    void **itemsPointer;
    uint64_t *mutationsPointer;
    uint64_t extra[5];
} FastEnumerationState;

uint64_t enumerateWithState(const void *const collection, FastEnumerationState *const state, void *iobuffer[], uint64_t length);

#ifndef foreach_start
#define foreach_start(type,variable,collection) \
{\
	FastEnumerationState ___state;\
	memset(&___state, 0, sizeof(FastEnumerationState));\
	ObjectRef ___buffer[16];\
	memset(&___buffer, 0, sizeof(___buffer));\
	int ___firstLoop = 1;\
	int64_t ___mutationsPointerValue;\
	uint64_t ___count;\
	while ((___count = enumerateWithState((collection), &___state, ___buffer, 16))) {\
		if (!___firstLoop && ___mutationsPointerValue != *___state.mutationsPointer) {\
			assert(0 && "Object mutated while iterating");\
		}\
		___firstLoop = 0;\
		___mutationsPointerValue = *___state.mutationsPointer;\
		type variable;\
		for (uint64_t ___index=0; ___index<___count; ___index++) {\
			variable = ___state.itemsPointer[___index];

#define foreach_end() }}}

#endif

#endif /* CObjects_foreach_h */
