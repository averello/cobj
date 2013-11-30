//
//  WString.h
//  CObjects
//
//  Created by George Boumis on 13/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_WString_h
#define CObjects_WString_h

#include <StringObject.h>
#include <wchar.h>

extern const void * WString;
extern const void * WStringClass;
typedef void * WStringRef;

const wchar_t *getWText(const void *const self);

void initWString() __attribute__ ((constructor));
void deallocWString() __attribute__ ((destructor));

#endif
