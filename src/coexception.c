//
//  coexception.c
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coexception.h>
#include <errno.h>
#include <assert.h>

#define COHANDLING 1
#define COHANDLED 2

__thread struct exception_context_t COExceptionThreadContext;

enum exception_context_list_item_state {
	ExceptionContextStateCode = COCODETRY,
	ExceptionContextStateHandling = COHANDLING,
	ExceptionContextStateHandled = COHANDLED,
	ExceptionContextStateFinally = COFINALLYCASE
};

struct exception_t {
	
};

void COExceptionMainConstructor() __attribute__ ((constructor));

void COExceptionMainConstructor() {
	memset((struct exception_context_t *)&COExceptionThreadContext, 0, sizeof(COExceptionThreadContext));
//	COExceptionThreadContext.count = 0;
//	COExceptionThreadContext.list = TAILQ_HEAD_INITIALIZER(COExceptionThreadContext.list);
//	TAILQ_HEAD(exception_context_list_head_t, exception_context_list_item_t) list = TAILQ_HEAD_INITIALIZER(list);
//	COExceptionThreadContext.list.tqh_last = &(COExceptionThreadContext.list).tqh_first;
//	TAILQ_INIT((struct exception_context_list_head_t *)&COExceptionThreadContext.list);
}

void COExceptionLink(struct exception_context_list_item_t *econtext) {
	/* Link the exception handler in the current list */
	COExceptionThreadContext.count++;
	
	struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
	econtext->entry.next = NULL;
	econtext->entry.prev = list->tail;
	if (NULL != list->tail)
		list->tail->entry.next = econtext;
	if (NULL == list->head)
		list->head = econtext;
	list->tail = econtext;

//	TAILQ_INSERT_TAIL(&COExceptionThreadContext.list, econtext, entry);
}


void COExceptionUnlink(struct exception_context_list_item_t *econtext) {
	/* If no exception occured then call finally */
	if (econtext->state == ExceptionContextStateCode || econtext->state == ExceptionContextStateHandled) {
		econtext->state = ExceptionContextStateFinally;
		longjmp(econtext->context, ExceptionContextStateFinally);
	}
	/* If handling and the code did not call COHandle() then propagate the exception */
	else if (econtext->state == ExceptionContextStateHandling) {
		econtext->state = ExceptionContextStateFinally;
		longjmp(econtext->context, ExceptionContextStateFinally);
//		COHandle(econtext);
	}
	/* If returning from a finally block remove the handler */
	else if (econtext->state == ExceptionContextStateFinally) {
		if (econtext->handled == 0) {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_context_list_item_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
			
			struct exception_context_list_item_t *previous = econtext->entry.prev;//TAILQ_PREV(econtext, exception_context_list_head_t, entry);
			if (NULL == previous) {
				/* Exception with no handler in place */
				//TODO: print the exception
				abort();
			}
			longjmp(previous->context, econtext->exceptionNumber);
		}
		else {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_context_list_item_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
		}
	}
}

void CORaise(int exception) {
	struct exception_context_list_item_t *econtext = COExceptionThreadContext.list.tail;
	assert(econtext != NULL);
	
	/* Raises the exception */
	if (econtext->state == ExceptionContextStateCode) {
		econtext->state = ExceptionContextStateHandling;
		econtext->exceptionNumber = exception;
		longjmp((int *)econtext->context, exception);
	}
	assert(0);
}

void COHandle(struct exception_context_list_item_t *econtext) {
	/* Indicates that the excpetion was handled */
	econtext->state = ExceptionContextStateHandled;
	econtext->handled = 1;
}

int COValue(struct exception_context_list_item_t *econtext) {
	return 0;
//	return econtext->exception;
}
