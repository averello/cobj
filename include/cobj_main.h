//
//  cobj_main.h
//  CObjects
//
//  Created by George Boumis on 8/1/14.
//  Copyright (c) 2014 George Boumis. All rights reserved.
//

#ifndef CObjects_cobj_main_h
#define CObjects_cobj_main_h

#include <coexception.h>
#include <AutoreleasePool.h>

#define COBJ_MAIN_BEGIN() AutoreleasePoolRef ___mainAutoreleasePool = new(AutoreleasePool, NULL);
#if DEBUG
#define COBJ_MAIN_END() release(___mainAutoreleasePool); memory_management_print_stats();
#else
#define COBJ_MAIN_END() release(___mainAutoreleasePool);
#endif






#endif
