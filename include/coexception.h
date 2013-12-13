//
//  coexception.h
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_coexception_h
#define CObjects_coexception_h

#include <setjmp.h>
#include <sys/queue.h>

typedef struct exception_t COException;

struct exception_context_t {
	volatile int count;
	struct exception_context_list_head_t {
		struct exception_context_list_item_t *head;	/* first element */
		struct exception_context_list_item_t *tail;	/* last element */
	} list;
};

struct exception_context_list_item_t {
	jmp_buf context;
	int state;
	int exceptionNumber;
	int handled;
	COException *exception;
	struct {
		struct exception_context_list_item_t *next;	/* next element */
		struct exception_context_list_item_t *prev;	/* previous element */
	} entry;
};




extern __thread struct exception_context_t COExceptionThreadContext;

#define COFINALLYCASE -1
#define COCODETRY 0

#define COTRY \
{ \
	struct exception_context_list_item_t econtext; \
	memset(&econtext, 0, sizeof(econtext)); \
	COExceptionLink(&econtext); \
	switch(setjmp(econtext.context)) {\
		case COCODETRY:


#define COCATCH(exception) break; case exception:
#define COFINALLY break; case COFINALLYCASE:
#define COOTHER break; default:

#define COEND } \
	COExceptionUnlink(&econtext); \
}

#define COTHROW(exception) CORaise(exception)
#define COHANDLE() COHandle(&econtext)
#define COVALUE() COValue(&econtext)

void COExceptionLink(struct exception_context_list_item_t *econtext);
void COExceptionUnlink(struct exception_context_list_item_t *econtext);
void CORaise(int exception);
void COHandle(struct exception_context_list_item_t *econtext);
int COValue(struct exception_context_list_item_t *econtext);

#endif
