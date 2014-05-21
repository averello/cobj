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

#define CO_LOG_BACKTRACE() CO_LOG_BACKTRACE_N(128)

#define CO_LOG_BACKTRACE_N(n) \
{ /* Print the backtrace */ \
int size = (n), nptrs; \
char **strings; \
void *stackbuffer[size]; \
nptrs = backtrace(stackbuffer, size); \
\
strings = backtrace_symbols(stackbuffer, nptrs); \
if (NULL != strings) { \
	fprintf(stderr, "Backtrace:\n"); \
	for (int i=0; i<nptrs; i++) \
		fprintf(stderr, "\t%s\n", strings[i]); \
		free(strings); \
		} \
}

/*
 Each thread needs to have its own context
 This variable declares each thread context.
 */
__thread struct exception_handlers_thread_context_t COExceptionThreadContext;

/*!
 *  @enum exception_context_list_item_state
 *	@private
 *  @brief The exception's handler state.
 *  @ingroup ehm
 */
enum exception_context_list_item_state {
	ExceptionContextStateCode = COCODETRY,		/*!< the handle is executing the code */
	ExceptionContextStateHandling = COHANDLING,	/*!< an exception occured and the handler is handling the exception */
	ExceptionContextStateHandled = COHANDLED,	/*!< this exception was handled */
	ExceptionContextStateFinally = COFINALLYCASE/*!< the handle is executing the finaly case */
};


void COExceptionMainConstructor() __attribute__ ((constructor));
static void COExceptionDealloc(void *exception);
static void COExceptionPropagate(struct exception_handler_context_t *econtext);
static void COExceptionUnhandledException(COException *) __attribute__((noreturn));



/* Zero out the exception context on launch */
void COExceptionMainConstructor() {
	memset((struct exception_handlers_thread_context_t *)&COExceptionThreadContext, 0, sizeof(COExceptionThreadContext));
}

void COExceptionLink(struct exception_handler_context_t *econtext) {
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


void COExceptionUnlink(struct exception_handler_context_t *econtext) {
	/* If no exception occured then call finally */
	if (econtext->state == ExceptionContextStateCode || econtext->state == ExceptionContextStateHandled) {
		econtext->state = ExceptionContextStateFinally;
		longjmp(econtext->context, ExceptionContextStateFinally);
	}
	/* 
	 If handling and the code did not call COHandle() then propagate the exception
	 This means that the exception was not handled
	 */
	else if (econtext->state == ExceptionContextStateHandling) {
		/* If finally was already called then propagate */
		if (econtext->finally == 1) {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_handler_context_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
			
			/* Propage the exception */
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
		if (econtext->handled == 0 && econtext->exception != NULL) {
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_handler_context_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
			
			/* Propage the exception */
			COExceptionPropagate(econtext);
		}
		/* Remove the handler */
		else {
			/* Release the exception */
			if (econtext->exception)
				release(econtext->exception);
			econtext->exception = NULL;
			
			/* Unlink current handler go up to the chain */
			struct exception_context_list_head_t *list = &(COExceptionThreadContext.list);
			struct exception_handler_context_t *item =  list->tail;
			list->tail = item->entry.prev;
			if (list->tail == NULL)
				list->head = NULL;
			COExceptionThreadContext.count--;
		}
	}
}

static void COExceptionPropagate(struct exception_handler_context_t *econtext) {
	struct exception_handler_context_t *previous = econtext->entry.prev;//TAILQ_PREV(econtext, exception_context_list_head_t, entry);
	/* If there is no previous handler then provoke an error for unhandled exception */
	if (NULL == previous) {
		COExceptionUnhandledException(econtext->exception);
		/* Should never reach here because the COExceptionUnhandledException() 
		 normally never returns so terminate the process */
		assert(0), abort();
	}
	/* Go up the handler chain */
	previous->exception = econtext->exception;
	longjmp(previous->context, previous->exception->exception);
}

static void COExceptionUnhandledException(COException *exception) {
	/* Exception with no handler in place */
	fprintf(stderr, "Terminating due to uncaught exception ");
	/* Log the exception */
	COExceptionLog(exception);
	/* Log the backtrace */
	CO_LOG_BACKTRACE();
	/* Terminate the process */
	abort();
}

void CORaise(COException *exception) {
	/* It is not possible to raise a non-existent exception */
	if (exception == NULL) {
		fprintf(stderr, "Tried to raise NULL exception\n");
		/* Log the backtrace */
		CO_LOG_BACKTRACE();
		/* Terminate the process */
		abort();
	}
	
	/* If the exception number is a reserved value it is not possible to raise 
	 the exception */
	if (exception->exception == COFINALLYCASE || exception->exception == COCODETRY) {
		fprintf(stderr, "Tried to raise exception using a reserved value:%d\n", exception->exception);
		/* Log the backtrace */
		CO_LOG_BACKTRACE();
		/* Terminate the process */
		abort();
	}
		
	/* Retrieve the context */
	struct exception_handler_context_t *econtext = COExceptionThreadContext.list.tail;
	
	/* If there no context in place then the exception goes unhandled */
	if (econtext == NULL) {
		COExceptionUnhandledException(exception);
		/* Should never reach here so terminate the process */
		assert(0), abort();
	}
	
	/* Raises the exception */
	if (econtext->state == ExceptionContextStateCode) {
		/* Switch the state to Handling */
		econtext->state = ExceptionContextStateHandling;
		/* Attach the exception to the context */
		econtext->exception = exception;
		longjmp(econtext->context, exception->exception);
	}
	/* If an exception raises in a catch clause */
	else if (econtext->state == ExceptionContextStateHandled) {
		/* If the finally block was not executed */
		if (econtext->finally == 0) {
			econtext->state = ExceptionContextStateHandling;
			econtext->finally = 1;
			
			/* Associate the new exception */
			release(econtext->exception);
			econtext->exception = exception;
			econtext->state = ExceptionContextStateHandling;
			/* And jump to the finally case */
			longjmp(econtext->context, ExceptionContextStateFinally);
		}
		else {
			/* If there is no finally or it is already executed then propagate 
			 the exception up in the chain */
			COExceptionPropagate(econtext);
		}
	}
	/* Should not reach here */
	assert(0);
}

void COHandle(struct exception_handler_context_t *econtext) {
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

COException *COExceptionGetCurrent(struct exception_handler_context_t *econtext) {
	if (econtext == NULL) return errno = EINVAL, (COException *)NULL;
	return econtext->exception;
}

COException *COExceptionAllocate(const int exceptionNumber, const char *name, const char *reason) {
	if (exceptionNumber == COFINALLYCASE || exceptionNumber == COCODETRY) return errno = EINVAL, (COException *)NULL;
	
	COException *exception = MEMORY_MANAGEMENT_ALLOC(sizeof(struct exception_t));
	if (NULL == exception) return errno = ENOMEM, (COException *)NULL;
	
	MEMORY_MANAGEMENT_ATTRIBUTE_SET_DEALLOC_FUNCTION(exception, COExceptionDealloc);
	
	if (NULL != name)
		exception->name = strdup(name);
	if (NULL != reason)
		exception->reason = strdup(reason);
	
	int *pExceptionNumber = (int *)&(exception->exception);
	*pExceptionNumber = exceptionNumber;
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
	/* If null then return */
	if (exception == NULL) return;
	fprintf(stderr, "'%s', value:%d reason:'%s'\n", COExceptionName(exception), COExceptionValue(exception), COExceptionReason(exception));
}
