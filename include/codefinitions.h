//
//  codefinitions.h
//  CObjects
//
//  Created by George Boumis on 11/3/2014.
//  Copyright (c) 2014 George Boumis. All rights reserved.
//

#ifndef CObjects_codefinitions_h
#define CObjects_codefinitions_h

#if !defined(CO_VISIBILITY_INTERNAL)
#define CO_VISIBILITY_INTERNAL __attribute__ ((visibility ("internal")))
#endif

#if !defined(CO_CONSTRUCTOR)
#define CO_CONSTRUCTOR __attribute__ ((constructor))
#endif

#if !defined(CO_DESTRUCTOR)
#define CO_DESTRUCTOR __attribute__ ((destructor))
#endif


/* Declare a class */
#if !defined(CO_DECLARE_CLASS)
#define CO_DECLARE_CLASS(class)  \
/*! A @ref class type. */ \
extern const void * class ; \
/*! A @ref class type */ \
extern const void * class##Class; \
/*! An @ref class reference. */ \
typedef void * class##Ref; \
/*! \
*  @fn void init##class() \
*  @relates class \
*  @brief A function that initializes @ref class and @ref class##Class. \
*  @details This function must be called once, before any instanciation of @ref String instance occur. \
*/ \
void init##class() CO_CONSTRUCTOR; \
/*! \
*  @fn void dealloc##class() \
*  @relates class \
*  @brief A function that deallocates any memory used by @ref init##class() counterpart. \
*  @details This function must be called once, after any instanciation of @ref class instances. After calling this method, instanciating a @ref class will terminate the application. \
*/ \
void dealloc##class() CO_DESTRUCTOR;
#endif

/* Declare the class structure */
#if !defined(CO_BEGIN_CLASS_TYPE_DECL)
#define CO_BEGIN_CLASS_TYPE_DECL(class,superclass) \
struct class { \
	const struct superclass isa;
#endif

#if !defined(CO_END_CLASS_TYPE_DECL)
#define CO_END_CLASS_TYPE_DECL };
#endif

/* Declare the class's class */
#if !defined(CO_BEGIN_CLASS_DECL)
#define CO_BEGIN_CLASS_DECL(class,superclass) \
struct class { \
	const struct superclass isa;
#endif

#if !defined(CO_END_CLASS_DECL)
#define CO_END_CLASS_DECL };
#endif



/* Declare internal class */
#if !defined(CO_CLASS_TYPE_INTERNAL_DECL_DEALLOC)
#define CO_CLASS_TYPE_INTERNAL_DECL_DEALLOC(class,superclass,super,classOverrides,instanceOverrides,...) \
const void * class = NULL; \
const void * class##Class = NULL; \
void init##class() { \
	if ( ! class##Class ) { \
		class##Class = new(superclass, #class "Class", superclass, sizeof(struct class##Class), classOverrides, NULL); \
	} \
	if ( ! class ) { \
		class = new(class##Class, #class, super, sizeof(struct class), instanceOverrides, NULL); \
	} \
} \
void dealloc##class() { \
	if (class) {\
		release((void*)class); \
	} \
	if (CollectionClass) { \
		release((void*)class##Class); \
	} \
	class = NULL; \
	class##Class = NULL; \
	__VA_ARGS__; \
}
#endif

#if !defined(CO_CLASS_TYPE_INTERNAL_DECL_INIT)
#define CO_CLASS_TYPE_INTERNAL_DECL_INIT(class,superclass,super,classOverrides,instanceOverrides,initcalls,...) \
const void * class = NULL; \
const void * class##Class = NULL; \
void init##class() { \
	initcalls; \
	if ( ! class##Class ) { \
		class##Class = new(superclass, #class "Class", superclass, sizeof(struct class##Class), classOverrides, NULL); \
	} \
	if ( ! class ) { \
		class = new(class##Class, #class, super, sizeof(struct class), instanceOverrides, NULL); \
	} \
} \
void dealloc##class() { \
	if (class) {\
		release((void*)class); \
	} \
	if (CollectionClass) { \
		release((void*)class##Class); \
	} \
	class = NULL; \
	class##Class = NULL; \
	__VA_ARGS__; \
}
#endif

#if !defined(CO_OVVERRIDE)
#define CO_OVERRIDE(...) __VA_ARGS__
#endif




#endif
