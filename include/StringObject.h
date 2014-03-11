/*!
 *  @file StringObject.h
 *  @brief @ref String definitions and methods.
 *  CObjects
 *
 *  Created by @author George Boumis
 *  @date 9/2/13.
 *  @copyright Copyright (c) 2013 George Boumis. All rights reserved.
*/

#ifndef CObjects_String_h
#define CObjects_String_h

#include <Object.h>
#include <coint.h>
#include <corange.h>

/*! A @ref String type. */
extern const void * String;

/*! A @ref StringClass type */
extern const void * StringClass;
/*! An @ref String reference. */
typedef void * StringRef;

/*!
 *  @enum SComparisonResult
 *  @brief Describes the three results of a comparison operation between objects.
 *  @relates String
 */
enum SComparisonResult {
	SAscending=-1, /*!< When the first object is less than the second */
	SSame=0,				/*!< When the first object is equal to the second */
	SDescending=1		/*!< When the first object is greater than the second */
};
/*!
 *  @typedef enum SComparisonResult SComparisonResult;
 */
typedef enum SComparisonResult SComparisonResult;

/*!
 *  @enum SStringComparingOptions
 *  @brief A bitmask for comparing options.
 *  @relates String
 */
enum SStringComparingOptions {
	SStringComparingOptionCaseInsensitiveSearch =	(1<<0),	/*!< A case-insensitive search. */ // = 0
	SStringComparingOptionLiteralSearch =					(1<<1),	/*!< Exact character-by-character equivalence. */ // = 2
	SStringComparingOptionLocalizedCompare =			(1<<2),	/*!< A localized compare */ // = 4
};
/*!
 *  @typedef enum SStringComparingOptions SStringComparingOptions;
 */
typedef enum SStringComparingOptions SStringComparingOptions;

/* API */

/*!
 *  @fn void initString()
 *  @relates String
 *  @brief A function that initializes @ref String and @ref StringClass.
 *  @details This function must be called once, before any instanciation of @ref String instance occur.
 */
void initString() __attribute__ ((constructor));

/*!
 *  @fn void deallocString()
 *  @relates String
 *  @brief A function that deallocates any memory used by @ref initString() counterpart.
 *  @details This function must be called once, after any instanciation of @ref String instances. After calling this method, instanciating a @ref String will terminate the application.
 */
void deallocString() __attribute__ ((destructor));

/*!
 *  @fn StringRef newStringWithFormat(const void *const _class, const void *format, ...)
 *  @relates String
 *  @brief A constructor that creates a new instance of @ref String (or a subclass of) with a given format. The format shall be equivalent to what is described in man 3 printf.
 *  @param[in] class the class of the instance demanded.
 *  @param[in] format the format.
 *  @param ... the variadic list of arguments wanted by the format.
 *  @returns a new @ref String instance with the specified format.
 */
StringRef newStringWithFormat(const void *const class, const void *format, ...);

/*!
 *  @method
 *  @relates String
 *  @brief A method that returns the @a C string representation of a @ref String instance.
 *  @param[in] self the @ref String (or a subclass) instace.
 *  @returns a pointer to a @C string.
 */
const char *getStringText(const void *const self);

/*!
 *  @method
 *  @relates String
 *  @brief A method that returns the length in number of characters in the receiver.
 *  @param[in] self the @ref String (or a subclass) instace.
 *  @returns the number of characters in the receiver.
 */
UInteger getStringLength(const void *const self);

/*!
 *  @method
 *  @relates String
 *  @brief A method that returns the character at a given position.
 *  @param[in] self the @ref String (or a subclass) instace.
 *  @param[out] character upon return the character at the position given by @a index. If the call failes @a character is left untouched.
 *  @param[in] index the index of the character to retrieve. The index must not be outside the bounds of the receiver.
 *  @returns 0 on completion. If index lies outside the bounds of the receiver then -1 is returned and @a errno is positioned to @a EINVAL.
 */
int characterAtIndex(const void *const self, void *const character, UInteger index);

/*!
 *  @method
 *  @relates String
 *  @brief A method that copies characters from a given range in the receiver into a given buffer.
 *  @details This method does not null-terminate the @a buffer. Upon successful completion exactly @a range.length number of characters are written in @a buffer.
 *  @param[in] self the @ref String (or a subclass) instace.
 *  @param[out] buffer the buffer to fill. Upon return, contains the characters from the receiver. buffer must be large enough to contain the characters in the range @a range (aRange.length*sizeof(unichar)). If the call failes @a buffer is left untouched.
 *  @param[in] range the range of characters to retrieve. The range must not exceed the bounds of the receiver.
 *  @returns 0 on completion. If range lies outside the bounds of the receiver then -1 is returned and @a errno is positioned to @a EINVAL.
 */
int getCharactersInRange(const void *const self, void *const restrict buffer, Range range);

/*!
 *  @method
 *  @relates String
 *  @brief A method compares one string with another string.
 *  @param[in] self the receiver @ref String (or a subclass) instace.
 *  @param[in] other the other @ref String (or a subclass) instace. The string to use in the comparison.
 *  @returns Returns an @ref SComparisonResult value that indicates the lexical ordering of the comparison.
 */
SComparisonResult compare(const void *const self, const void *const other);

/*!
 *  @method
 *  @relates String
 *  @brief A method compares one string with another string.
 *  @param[in] self the receiver @ref String (or a subclass) instace.
 *  @param[in] other the other @ref String (or a subclass) instace. The string to use in the comparison.
 *  @param[in] options options for the search - you can combine any of the following using a @a C bitwise @a OR operator.
 *  @returns Returns an @ref SComparisonResult value that indicates the lexical ordering of the comparison. @ref SAscending if receiver precedes @a other in lexical ordering, @ref SSame if the receiver and @a other are equivalent in lexical value, and @ref SDescending if the receiver follows @a other.
 */
SComparisonResult compareWithOptions(const void *const self, const void *const other, SStringComparingOptions options);

/*!
 *  @method
 *  @relates String
 *  @brief A method that creates a new @ref String that has trimmed whitespace from the beginning and end of the receiver.
 *  @details Whitespace for this function includes space characters, tabs, newlines, carriage returns, and any similar characters that do not have a visible representation.
 *  @param[in] self the receiver @ref String (or a subclass) instace.
 *  @param[in] other the other @ref String (or a subclass) instace.
 *  @returns the trimmed @ref String instance.
 */
StringRef copyStringByTrimmingSpaces(const void *const self);

/*!
 *  @method
 *  @relates String
 *  @brief A method that creates a new @ref String that is the catenation of the receiver with another string.
 *  @param[in] self the receiver @ref String (or a subclass) instace.
 *  @param[in] other the other @ref String (or a subclass) instace.
 *  @returns the concatenation of the receiver and the other object as a new @ref String.
 */
StringRef copyStringByAppendingString(const void *restrict const self, const void *restrict const other);


#endif
