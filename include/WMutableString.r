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

#include <stdint.h>

struct WMutableString {
	struct MutableString isa;
	off_t offset;
	uint64_t capacity;
};

struct WMutableStringClass {
	struct MutableStringClass isa;
};


#endif
