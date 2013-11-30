/*!
 *  @file new.h
 *  @brief Define the @a new() and @a delete() functions.
 *  CObjects
 *
 *  Created by @author George Boumis
 *  @date 9/2/13.
 *  @copyright Copyright (c) 2013 George Boumis. All rights reserved.
*/

#ifndef CObjects_new_h
#define CObjects_new_h

/*!
 *  @fn void * new (const void *const restrict _class, ...);
 *  @relates Object
 *  @brief This funciton creates a new instance of the given class.
 *  @param[in] class the class type of the instance to be created.
 *  @returns a @ref ObjectRef pointer to a newly allocated instance or NULL on error.
 */
void * new (const void *const restrict class, ...);

/*!
 *  @fn void delete (void * self)
 *  @relates Object
 *  @brief This funciton deallocates an instance by calling its destructor. Then frees the memory used.
 *  @param[in] self the instance of type @ref Object (or a subclass).
 */
void delete (void * self);


#endif
