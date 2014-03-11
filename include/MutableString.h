//
//  MutableString.h
//  CObjects
//
//  Created by averello on 6/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableString_h
#define CObjects_MutableString_h

extern const void * MutableString;
extern const void * MutableStringClass;
typedef void * MutableStringRef;

void initMutableString() __attribute__ ((constructor));
void deallocMutableString() __attribute__ ((destructor));

void appendString(void *const self, const void *const other);
void appendFormat(void *const self, char *format, ...);
void setString(void *const self, const void *const other);
void setMutableStringLength(void *const self, size_t capacity);
int insertStringAtMutableStringIndex(void *const self, const void *const other, unsigned long index);
int deleteMutableStringCharactersInRange(void *const self, CORange range);

//int replaceCharactersInRangeWithString(void *const self, CORange range, const void *const other);
//int replaceOccurencesOfStringWithStringOptionsAndRange(void *const self, const void *const other, CORange range, SStringComparingOptions options);

#endif
