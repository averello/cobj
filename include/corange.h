//
//  corange.h
//  CObjects
//
//  Created by George Boumis on 11/3/2014.
//  Copyright (c) 2014 George Boumis. All rights reserved.
//

#ifndef CObjects_corange_h
#define CObjects_corange_h

#include <coint.h>

/*!
 *  @struct Range StringObject.h
 *  @relates String
 *  @brief A structure describing a range.
 *  @details This structure is used by @ref String instances to describe portions of a string or substrings.
 */
typedef struct _Range {
    UInteger location; /*!< The start location of the range */
    UInteger length;	/*!< The length of the range begining from @a location */
} Range;

/*!
 *  @fn Range MakeRange(UInteger loc, UInteger len)
 *  @brief A helper function to create Range structures.
 *  @relates String
 *  @param[in] location the starting location of the range.
 *  @param[in] length the length of the range.
 *  @returns a new @ref Range with the specified attributes.
 */
inline __attribute__((always_inline)) Range MakeRange(UInteger location, UInteger length) {
    Range range;
    range.location = location;
    range.length = length;
    return range;
}

/*!
 *  @fn UInteger MaxRange(Range range)
 *  @brief A helper function that returns the end of the range (location + length).
 *  @relates String
 *  @param[in] range the range.
 *  @returns the max range (range.location + range.length).
 */
inline __attribute__((always_inline)) UInteger MaxRange(Range range) {
	return (range.location + range.length);
}

/*!
 *  @fn bool LocationInRange(UInteger location, Range range)
 *  @brief A helper function indiqating if the given location is in the given range.
 *  @relates String
 *  @param[in] location the location.
 *  @param[in] range the range.
 *  @returns a @a C boolean.
 */
inline __attribute__((always_inline)) bool LocationInRange(UInteger location, Range range) {
    return (location - range.location < range.length);
}

/*!
 *  @fn int EqualRanges(Range range1, Range range2)
 *  @brief A helper function indiqating if two given ranges are equal.
 *  @relates String
 *  @param[in] range1 the first range.
 *  @param[in] range2 the second range.
 *  @returns a @a C boolean.
 */
inline __attribute__((always_inline)) bool EqualRanges(Range range1, Range range2) {
    return (range1.location == range2.location
			&& range1.length == range2.length);
}

#endif
