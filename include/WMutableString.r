//
//  WMutableString.r
//  CObjects
//
//  Created by George Boumis on 30/11/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_WMutableString_r
#define CObjects_WMutableString_r

#include <MutableString.h>
#include <MutableString.r>

CO_BEGIN_CLASS_TYPE_DECL(WMutableString,MutableString)
	off_t offset;
	UInteger capacity;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(WMutableStringClass,MutableStringClass)
CO_END_CLASS_DECL


#endif
