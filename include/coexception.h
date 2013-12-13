//
//  coexception.h
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

/*!
 *  @file coexception.h
 *  @brief Exception Handling Module.
 *  @details This module permits to handle application exceptions.
 *
 *  Created by @author George Boumis
 *  @date 2013/12/13.
 *	@version 1.0
 *  @copyright Copyright (c) 2013 George Boumis <developer.george.boumis@gmail.com>. All rights reserved.
 *
 *  @defgroup ehm Exception Handling Module
 *
 *	mainpage Exception Handling Module Documentation.
 *	section intro_sec Introduction
 *	A little library in `C` for managing application emitted exceptions. See @ref ehm for documentation of this library.
 */

#ifndef CObjects_coexception_h
#define CObjects_coexception_h

#include <setjmp.h>
#include <sys/queue.h>

/*!
 *  @typedef struct exception_t COException
 *  @brief An application exception.
 *  @ingroup ehm
 *
 *
 *	Overview
 *	========
 *  'COException' is used to implement exception handling and contains information about an exception. An exception is a special condition that interrupts the normal flow of program execution. Each application can interrupt the program for different reasons. For example, one application might interpret saving a file in a directory that is write-protected as an exception. In this sense, the exception is equivalent to an error. Another application might interpret the user’s key-press (for example, Control-C) as an exception: an indication that a long-running process should be aborted.
 */
typedef struct exception_t COException;

/*!
 *  @struct exception_t
 *  @brief The exception structure.
 *  @ingroup ehm
 *	@details Contains all necessary information needed to represent an exception. You can allocate statically your exception on the stack and use @ref CORAISE to raise them.
 */
struct exception_t {
	const char *name; /*!< the exception's name */
	const char *reason; /*!< the exception's reason */
	int exception; /*!< the exception's number */
};

/*!
 *  @struct exception_handlers_context_t
 *  @brief The exception handlers context structure.
 *  @ingroup ehm
 *	@details Contains all necessary information needed by a thread to manage the exception handlers chain.
 */
struct exception_handlers_thread_context_t {
	volatile int count; /*!< the size of the list */
	struct exception_context_list_head_t {
		struct exception_handler_context_t *head;	/* first element */
		struct exception_handler_context_t *tail;	/* last element */
	} list; /*!< the stack of exceptions handlers */
};

/*!
 *  @struct exception_handler_context_t
 *  @brief The exception handler context structure.
 *  @ingroup ehm
 *	@details Contains all necessary information needed by a try-clause to handle or raise un exception.
 */
struct exception_handler_context_t {
	jmp_buf context; /*!< the register context */
	int state; /*!< the state of this handler */
	int handled; /*!< indicating whether the pending exception was handled by the application  */
	int finally; /*!< indicating whether the finally-clause was called for this handler */
	COException *exception; /*!< the exception associated to this exception handler */
	struct {
		struct exception_handler_context_t *next;	/* next element */
		struct exception_handler_context_t *prev;	/* previous element */
	} entry; /*!< the entry in the exception handlers' chain */
};

extern __thread struct exception_handlers_thread_context_t COExceptionThreadContext;

#define COFINALLYCASE -1
#define COCODETRY 0

/*!
 *  @define COTRY
 *  @brief The try-clause
 *  @ingroup ehm
 *	~~~~~~~~~~~~~~~~~{.c}
 *	COTRY {
 *		SomeFunctionThatThrowsAnException();
 *	}
 *	COCATCH(42) {
 *		// Handle the exception and then call COHANDLE() to indicate that you handled the exception.
 *		COHANDLE();
 *	}
 *	COFINALLY {
 *		// This block is called after an exception catch or after the try-clause if no exception were thrown
 *	}
 *	COEND
 *	~~~~~~~~~~~~~~~~~
 */
#define COTRY \
{ \
	struct exception_handler_context_t econtext; \
	memset(&econtext, 0, sizeof(econtext)); \
	COExceptionLink(&econtext); \
	switch(setjmp(econtext.context)) {\
		case COCODETRY:


/*!
 *  @define COCATCH(exception)
 *  @brief The catch-clause
 *  @ingroup ehm
 *	@details Catches an exception (int) thrown by a try-clause (@ref COTRY).
 *	@param[in] exception the exception number to catch
 */
#define COCATCH(exception) break; case exception:

/*!
 *  @define COFINALLY
 *  @brief The finally-clause
 *  @ingroup ehm
 *	@details This block is called after an exception catch or after the try-clause if no exception were thrown
 */
#define COFINALLY break; case COFINALLYCASE:

/*!
 *  @define COOTHER
 *  @brief A catch-clause when you want to catch unexpected exceptions, that is, when you have no catch-clause (@ref COCATCH) trying the exception that was thrown
 *  @ingroup ehm
 *	@details This block is called after an exception catch or after the try-clause if no exception were thrown
 */
#define COOTHER break; default:

/*!
 *  @define COEND
 *  @brief Used to terminate a @ref COTRY block.
 *  @ingroup ehm
 */
#define COEND } \
	COExceptionUnlink(&econtext); \
}

/*!
 *  @define COTHROW
 *  @brief Creates and throws/raises an exception with the given parameters.
 *  @ingroup ehm
 *	@param[in] exception the exception number to throw
 *	@param[in] name the name of the exception. This parameter is copied.
 *	@param[in] reason the reason this exception was thrown. This parameter is copied
 *	@warning This function **never** returns. As a consequence if you allocated the `name` and `reason` parameters with malloc your code can leak.
 */
#define COTHROW(exception, name, reason) CORaise(COExceptionAllocate(exception, name, reason))

/*!
 *  @define CORAISE
 *  @brief Throws/raises an exception
 *  @ingroup ehm
 *	@param[in] exception the exception number to throw
 *	@warning If the exception is not `NULL` then this function **never** returns. As a consequence if you allocated the `name` and `reason` parameters with malloc your code can leak.
 */
#define CORAISE(exception) CORaise(exception)

/*!
 *  @define COHANDLE()
 *  @brief Indicates to the exception handler that the exception was handled.
 *  @ingroup ehm
 *	@details If you do not call @ref COHANDLE() then the exception library considers that the exception was not handled and searches in the exception handler chain the next exception handler. This is called "exception propagation" or "stack unwinding". You should call this macro only inside a catch-clause block.
 *	@warning You should always call this macro when you want to indicate that the exception was correctly handled.
 */
#define COHANDLE() COHandle(&econtext)


void COExceptionLink(struct exception_handler_context_t *econtext);
void COExceptionUnlink(struct exception_handler_context_t *econtext);
void CORaise(COException *exception);
void COHandle(struct exception_handler_context_t *econtext);
COException *COExceptionAllocate(int exception, const char *name, const char *reason);

/* Public API */

/*!
 *  @define COCurrentException()
 *  @brief Returns the exception that was thrown.
 *  @ingroup ehm
 *	@details This macro should be used **only** in the context of an catch-clause.
 *	@returns the @ref COException that was catched.
 */
#define COCurrentException() COExceptionGetCurrent(&econtext)

/*!
 *  @fn COException *COExceptionGetCurrent(struct exception_handler_context_t *econtext)
 *  @brief Use @ref COCurrentException instead.
 *  @ingroup ehm
 *	@private
 */
COException *COExceptionGetCurrent(struct exception_handler_context_t *econtext);

/*!
 *  @fn int COExceptionValue(COException *exception)
 *  @brief Returns the exception number of the exception
 *  @ingroup ehm
 *	@param[in] exception the exception
 *	@returns the exception number of the exception
 */
int COExceptionValue(COException *exception);

/*!
 *  @fn const char *COExceptionName(COException *exception)
 *  @brief Returns the exception's name
 *  @ingroup ehm
 *	@param[in] exception the exception
 *	@returns the exception's name
 */
const char *COExceptionName(COException *exception);

/*!
 *  @fn const char *COExceptionReason(COException *exception)
 *  @brief Returns the exception's reason
 *  @ingroup ehm
 *	@param[in] exception the exception
 *	@returns the exception's reason
 */
const char *COExceptionReason(COException *exception);

/*!
 *  @fn void COExceptionLog(COException *exception)
 *  @brief Logs the exception
 *  @ingroup ehm
 *	@details Logs to stderr the exceptions internal state in the format: 'name' value:v reason:'reason'
 *	@param[in] exception the exception
 */
void COExceptionLog(COException *exception);

#endif
