//
//  testFastEnumeration.c
//  CObjects
//
//  Created by George Boumis on 10/12/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <cobj.h>

int main () {
	{
		MutableArrayRef strings = new(MutableArray, NULL);
		for (unsigned long i=0; i<10; i++) {
			StringRef firstString = newStringWithFormat(String, "string %lu", i);
			addObject(strings, firstString);
			release(firstString);
		}
		
		foreach_start(StringRef, string, strings) {
			 puts(getStringText(string));
		} foreach_end()
		
		release(strings);
	}
	return EXIT_SUCCESS;
}
