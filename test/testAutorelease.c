//
//  testAutorelease.c
//  CObjects
//
//  Created by George Boumis on 13/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <cobj.h>
#include <memory_management/memory_management.h>

int main () {
	COBJ_MAIN_BEGIN()
//	AutoreleasePoolRef autoreleasePool = new(AutoreleasePool, NULL);
	AutoreleasePoolRef autoreleasePool2 = new(AutoreleasePool, NULL);
	StringRef string = new(String, "Test String", NULL);
	autorelease(string);
	COBJ_MAIN_END()
	return 0;
}