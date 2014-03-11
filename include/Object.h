/*!
 * @file Object.h
 *	@brief @ref Object and @ref Class definitions and some @ref Object method's.
 * CObjects
 *
 * Created by @author George Boumis
 * @date 9/2/13.
 *  @copyright Copyright (c) 2013 George Boumis. All rights reserved.
*/

#ifndef CObjects_object_h
#define CObjects_object_h

#undef retain
#undef release

#include <stdint.h>

/*! An @ref Object type. */
extern const void *const Object;
/*! An @ref Object reference. */
typedef void * ObjectRef;

/*! @ref Class is the type of a @ref Class class (and it is a subclass of @ref Object). */
extern const void *const Class;
/*! A @ref Class reference */
typedef void * ClassRef;


/*!
 *  @method
 *  @relates Object
 *  @brief A method that returns the superclass of the current @ref Object instance.
 *  @param[in] self the object of which the superclass is asked, of type @ref Object.
 *  @return a @ref ClassRef pointer.
 */
const void * superclass (const void * self);

/*! 
 *  @method
 *  @relates Object
 *  @brief A method that returns the class's size (in bytes) of the receiver
 *  @param[in] self the instance of type @ref Object.
 *  @return the size (in bytes) as an size_t type.
 */
uint32_t sizeOf(const void * self);

/*! 
 *  @method
 *  @relates Object
 *  @brief A method that returns the class of the receiver.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @ref ClassRef pointer.
 */
const void * classOf(const void * self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method that indiquates wether the receiver is an direct instance of the given class.
 *  @param[in] self the instance of type @ref Object.
 *  @param[in] class the @ref Class to test with.
 *  @return a @a C boolean.
 */
int instanceOf (const void * self, const void * class);

/*!
 *  @method
 *  @relates Object
 *  @brief A method that indiquates wether the receiver is an instance of the given class or a subclass of the given class.
 *  @param[in] self the instance of type @ref Object.
 *  @param[in] class the @ref Class to test with.
 *  @return a @a C boolean.
 */
int isSubclassOf (const void * self, const void * class);

/*!
 *  @method
 *  @relates Object
 *  @brief A method that returns (as a C string) the name of the class of the receiver.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @a C boolean.
 */
const char *getClassName(const void * self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method that returns a copy of the receiver.
 *  @details The name of the method contains the word @a copy so the caller of this function is responsible for the memory of the returned object.
 *  @param[in] self the instance of type @ref Object.
 *  @return an @ref ObjectRef pointing a new allocated object that if passed to @ref equals it returns @a true.
 */
void * copy(const void *const self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method indiqating if two @ref Objects are equal.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @a C boolean.
 */
int equals(const void * self, const void *const other);

/*!
 *  @method
 *  @relates Object
 *  @brief A method giving the hash code of the receiver.
 *  @param[in] self the instance of type @ref Object.
 *  @return the hash code of the given instance.
 */
int hash (const void *const self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method returning a @ref String representation of the receiver.
 *  @details the caller is responsible for the memory return from this method.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @ref StringRef describing the receiver.
 */
void * copyDescription (const void *const self);

/* Memory Management */

/*!
 *  @method
 *  @relates Object
 *  @brief A method incrementing the reference count of an @ref Object.
 *  @details This method should be called each time a caller is interested in an @ref Object.
 *  @param[in] self the instance of type @ref Object.
 *  @return @a self (as an @ref ObjectRef).
 */
void * retain (void *const self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method decrementing the reference count of an @ref Object.
 *  @details This method should be called each time a caller is no longer interested in an @ref Object. This method is responsible for deallocating/freeing an @ref Object's memory when the reference counter goes to 0.
 *  @param[in] self the instance of type @ref Object.
 */
void release (void *const self);

/*!
 *  @method
 *  @relates Object
 *  @brief Decrements the receiver’s retain count at the end of the current autorelease pool block.
 *  @details This method should be called each time a function wants to transfer ownership to the caller.
 *  @param[in] self the instance of type @ref Object.
 */
void *autorelease (void *const self);

/*!
 *  @method
 *  @relates Object
 *  @brief A method returning the actual reference count of the receiver.
 *  @param[in] self the instance of type @ref Object.
 *  @return the number of @ref retain calls to this instance.
 */
unsigned long retainCount (const void *const self);

#endif

