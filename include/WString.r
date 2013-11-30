//
//  WString.r
//  CObjects
//
//  Created by George Boumis on 13/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_WString_r
#define CObjects_WString_r

#include <StringObject.h>
#include <StringObject.r>

struct WString {
	struct String isa;
};

struct WStringClass {
	struct StringClass isa;
};

#endif
