//
//  WMutableString.h
//  CObjects
//
//  Created by George Boumis on 30/11/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_WMutableString_h
#define CObjects_WMutableString_h

#include <MutableString.h>
#include <wchar.h>

extern const void * WMutableString;
extern const void * WMutableStringClass;
typedef void * WMutableStringRef;

void initWMutableString() __attribute__ ((constructor));
void deallocWMutableString() __attribute__ ((destructor));

const wchar_t *getWText(const void *const self);

#endif
