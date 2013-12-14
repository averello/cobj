/*!
//  @file Object.r
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  @Copyright (c) 2013 George Boumis. All rights reserved.
*/

#ifndef CObjects_object_r
#define CObjects_object_r

#include <pthread.h>

/*!
 *  @class Object Object.r
 *  @private
 *  @brief The basic structure/class that describes an @ref Object type.
 *  @details This class contains a pointer to a @ref Class instance, a reference counter, and a @a pthread_mutex_t for internal purposes.
 */
struct Object {
	/*!
	 *  @member class
	 *  @protected
	 *  @brief A pointer to a @ref Class describing this instances behaviour.
	 */
	const struct Classs * class;
	/*!
	 *  @member reatainCount
	 *  @protected
	 *  @brief The reference counter of this instance.
	 */
	//unsigned long retainCount;
	/*!
	 *  @member guardian
	 *  @protected
	 *  @brief Used for thread safe reference counter operations.
	 */
	//pthread_mutex_t guardian;
};

/*!
 *  @typedef typedef void * (* ctor) (void * self, va_list * app)
 *  @private
 *  @brief The form of an @ref Object @ref constructor.
 */
typedef void * (* ctor) (void * self, va_list * app);

/*!
 *  @typedef typedef void * (* dtor) (void * self)
 *  @private
 *  @brief The form of an @ref Object @ref destructor.
 */
typedef void * (* dtor) (void * self);

/*!
 *  @typedef typedef typedef void * (* cpy) (const void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref copy method.
 */
typedef void * (* cpy) (const void *const self);

/*!
 *  @typedef typedef int (* eq) (const void * self, const void * other)
 *  @private
 *  @brief The form of an @ref Object's @ref equals method.
 */
typedef int (* eq) (const void * self, const void * other);

/*!
 *  @typedef typedef int (* hsh) (const void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref hash method.
 */
typedef int (* hsh) (const void *const self);

/*!
 *  @typedef typedef void * (* desc) (const void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref copyDescription method
 */
typedef void * (* desc) (const void *const self);

/*!
 *  @typedef typedef void * (* ret) (void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref retain method
 */
typedef void * (* ret) (void *const self);

/*!
 *  @typedef typedef void (* rel) (void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref release method
 */
typedef void (* rel) (void *const self);

/*!
 *  @typedef typedef unsigned long (* retc) (const void *const self)
 *  @private
 *  @brief The form of an @ref Object's @ref retainCount method
 */
typedef unsigned long (* retc) (const void *const self);


typedef void *(*autor) (void *const self);

/*!
 *  @class Class Object.r
 *  @private
 *  @brief The basic structure/class that describes an @ref Class type.
 *  @details The instance of this class is an @ref Object. This class contains the @ref Object structure, the name of the class as a @a C string, a pointer to the super class, the size of an instance of this class. It contains also functions pointers to the class's methods.
 */
struct Classs {
	/*!
	 *  @member isa
	 *  @protected
	 *  @brief The object type this class.
	 */
	struct Object isa;
	
	/*!
	 *  @member class_name
	 *  @protected
	 *  @brief The class's name as a @a C string.
	 */
	const char *class_name;
	
	/*!
	 *  @member super
	 *  @protected
	 *  @brief The pointer to the  super class's @ref Class.
	 */
	const struct Classs * super;
	
	/*!
	 *  @member size
	 *  @protected
	 *  @brief The size for this class' instances.
	 */
	size_t size;
	
	/* 
	 Methods 
	 */
	
	/*!
	 *  @member constructor
	 *  @protected
	 *  @brief The function pointer to the @ref constructor of this class.
	 */
	ctor constructor;
	
	/*!
	 *  @member destructor
	 *  @protected
	 *  @brief The function pointer to the @ref destructor of this class.
	 */
	dtor destructor;

	/*!
	 *  @member copy
	 *  @protected
	 *  @brief The function pointer to the @ref copy method of this class.
	 */
	cpy copy;
	
	/*!
	 *  @member copy
	 *  @protected
	 *  @brief The function pointer to the @ref equals method of this class.
	 */
	eq equals;

	/*!
	 *  @member hash
	 *  @protected
	 *  @brief The function pointer to the @ref hash method of this class.
	 */
	hsh hash;
	
	/*!
	 *  @member copyDescription
	 *  @protected
	 *  @brief The function pointer to the @ref copyDescription method of this class.
	 */
	desc copyDescription;
	
	
	/*
	 Memory management 
	 */
	
	/*!
	 *  @member retain
	 *  @protected
	 *  @brief The function pointer to the @ref retain method of this class.
	 */
	ret retain;
	
	/*!
	 *  @member release
	 *  @protected
	 *  @brief The function pointer to the @ref release method of this class.
	 */
	rel release;
	
	/*!
	 *  @member retainCount
	 *  @protected
	 *  @brief The function pointer to the @ref retainCount method of this class.
	 */
	retc retainCount;
	
	/*!
	 *  @member autorelease
	 *  @protected
	 *  @brief The function pointer to the @ref autorelease method of this class.
	 */
	autor autorelease;
};

/*!
 *  @protected
 *  @method void * super_constructor(const void * class, void * self, va_list * app)
 *  @relates Object
 *  @brief A method that calls the super class's constructor of a given class.
 *  @param[in] class the @ref Class which its super constructor will be called.
 *  @param[in] self the instance, of type @ref Object.
 *  @param[in] app the variadic list of arguments.
 *  @return a @ref ObjectRef pointer (probably) pointing to self.
 */
void * super_constructor(const void * class, void * self, va_list * app);

/*!
 *  @protected
 *  @method void * super_destructor(const void * class)
 *  @relates Object
 *  @brief A method that calls the super class's destructor.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @ref ObjectRef pointer (probably) pointing to self.
 */
void * super_destructor(const void * class, void * self);

/*!
 *  @protected
 *  @method void * constructor(void * self, va_list * app)
 *  @relates Object
 *  @brief An @ref Object @ref constructor. It's called by @ref new() to initialize a  newly created instance. This method can return another pointer than @a self. The return value can be @a NULL if initialization failed.
 *  @param[in] self the instance of type @ref Object.
 *  @param[in] app the variadic list of arguments.
 *  @return a @ref ObjectRef pointer (probably) pointing to self.
 */
void * constructor(void * self, va_list * app);

/*!
 *  @protected
 *  @method void * super_destructor(const void * class)
 *  @relates Object
 *  @brief An @ref Object @ref destructor. It's called by @ref delete() when destroying an instance. This method should free all memory allocated by the instance. Either @a NULL or a pointer suitable to be passed to free() is returned. An instance can return @a NULL and not deallocate any memory if it wants to stay alive.
 *  @param[in] self the instance of type @ref Object.
 *  @return a @ref ObjectRef pointer (probably) pointing to self.
 */
void * destructor(void * self);



void * Object_constructor (void * _self, va_list * app);
void * Object_destructor (void * _self);
void * Object_copy (const void * const _self);
int Object_equals (const void *const _self, const void *const _other);

int Object_hash (const void *const _self);

/* Memory management */
void * Object_retain (void *const _self);
void Object_release (void *const _self);
unsigned long Object_retainCount (const void *const _self);
void * Object_autorelease (void * _self);

void * Object_copyDescription (const void *const _self);

const void * super (const void *const _self);

#endif
