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
#include <execinfo.h>
#include <limits.h>
#include <memory_management/memory_management.h>

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
	const char *name;
	const char *reason;
	int exception;
};

void COExceptionMainConstructor() __attribute__ ((constructor));
static void COExceptionDealloc(void *exception);
static void COExceptionPropagate(struct exception_context_list_item_t *econtext);
static void COExceptionUnhandledException(COException *) __attribute__((noreturn));

void COExceptionMainConstructor() {
	memset((struct exception_context_t *)&COExceptionThreadContext, 0, sizeof(COExceptionThreadContext));
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
}


void COExceptionUnlink(struct exception_context_list_item_t *econtext) {
	/* If no exception occured then call finally */
	if (econtext->state == ExceptionContextStateCode || econtext->state == ExceptionContextStateHandled) {
		econtext->state = ExceptionContextStateFinally;
		longjmp(econtext->context, ExceptionContextStateFinally);
	}
	/* If handling and the code did not call COHandle() then propagate the exception */
	else if (econtext->state == ExceptionContextStateHandling) {
		/* If finally already called then propagate */
		if (econtext->finally == 1) {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_context_list_item_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
			
			COExceptionPropagate(econtext);
		}
		/* Otherwise call finally first */
		else {
			econtext->state = ExceptionContextStateFinally;
			longjmp(econtext->context, ExceptionContextStateFinally);
		}
	}
	/* If returning from a finally block */
	else if (econtext->state == ExceptionContextStateFinally) {
		/* Propagate if the exception is not handled */
		if (econtext->handled == 0) {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_context_list_item_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
			
			COExceptionPropagate(econtext);
		}
		/* Remove the handler */
		else {
			release(econtext->exception);
			econtext->exception = NULL;
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_context_list_item_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
		}
	}
}

static void COExceptionPropagate(struct exception_context_list_item_t *econtext) {
	struct exception_context_list_item_t *previous = econtext->entry.prev;//TAILQ_PREV(econtext, exception_context_list_head_t, entry);
	if (NULL == previous)
		COExceptionUnhandledException(econtext->exception);
	previous->exception = econtext->exception;
	longjmp(previous->context, previous->exception->exception);
}

static void COExceptionUnhandledException(COException *exception) {
	/* Exception with no handler in place */
	fprintf(stderr, "Terminating due to uncaught exception ");
	COExceptionLog(exception);
	int size = 128, nptrs;
	char **strings;
	void *stackbuffer[size];
	nptrs = backtrace(stackbuffer, size);
	
	strings = backtrace_symbols(stackbuffer, nptrs);
	if (NULL != strings) {
		fprintf(stderr, "Backtrace:\n");
		for (unsigned int i=0; i<nptrs; i++)
			fprintf(stderr, "\t%s\n", strings[i]);
		free(strings);
	}
	abort();
}

void CORaise(COException *exception) {
	struct exception_context_list_item_t *econtext = COExceptionThreadContext.list.tail;
//	assert(econtext != NULL);
	if (econtext == NULL)
		COExceptionUnhandledException(exception);
	
	/* Raises the exception */
	if (econtext->state == ExceptionContextStateCode) {
		econtext->state = ExceptionContextStateHandling;
		econtext->exception = exception;
		longjmp((int *)econtext->context, exception->exception);
	}
	else if (econtext->state == ExceptionContextStateHandled) {
		if (econtext->finally == 0) {
			econtext->state = ExceptionContextStateHandling;
			econtext->finally = 1;
			release(econtext->exception);
			econtext->exception = exception;
			econtext->state = ExceptionContextStateHandling;
			longjmp(econtext->context, ExceptionContextStateFinally);
		}
		else {
			COExceptionPropagate(econtext);
		}
	}
	assert(0);
}

void COHandle(struct exception_context_list_item_t *econtext) {
	/* Indicates that the excpetion was handled */
	econtext->state = ExceptionContextStateHandled;
	econtext->handled = 1;
}

int COExceptionValue(COException *exception) {
	if (exception == NULL) return errno = EINVAL, -1;
	return exception->exception;
}

const char *COExceptionName(COException *exception) {
	if (exception == NULL) return errno = EINVAL, NULL;
	return exception->name;
}

const char *COExceptionReason(COException *exception) {
	if (exception == NULL) return errno = EINVAL, NULL;
	return exception->reason;
}

COException *COExceptionGetCurrent(struct exception_context_list_item_t *econtext) {
	assert(econtext != NULL);
	if (econtext == NULL) return errno = EINVAL, (COException *)NULL;
	return econtext->exception;
}

COException *COExceptionAllocate(int exceptionNumber, const char *name, const char *reason) {
	if (exceptionNumber == COFINALLYCASE) return errno = EINVAL, (COException *)NULL;
	
	COException *exception = MEMORY_MANAGEMENT_ALLOC(sizeof(struct exception_t));
	if (NULL == exception) return errno = ENOMEM, (COException *)NULL;
	
	MEMORY_MANAGEMENT_ATTRIBUTE_SET_DEALLOC_FUNCTION(exception, COExceptionDealloc);
	
	if (NULL != name)
		exception->name = strdup(name);
	if (NULL != reason)
		exception->reason = strdup(reason);
	exception->exception = exceptionNumber;
	return exception;
}

static void COExceptionDealloc(void *_exception) {
	if (NULL == _exception) return;
	
	COException *exception = _exception;
	if (NULL != exception->name)
		free((void *)exception->name), exception->name = NULL;
	if (NULL != exception->reason)
		free((void *)exception->reason), exception->reason = NULL;
}

void COExceptionLog(COException *exception) {
	fprintf(stderr, "'%s', value:%d reason:'%s'\n", COExceptionName(exception), COExceptionValue(exception), COExceptionReason(exception));
}
