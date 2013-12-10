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
		
		{
			FastEnumerationState ___state;
			__builtin___memset_chk (&___state, 0, sizeof(FastEnumerationState), __builtin_object_size (&___state, 0));
			ObjectRef ___buffer[16];
			memset(___buffer, 0, sizeof(___buffer));
			int ___firstLoop = 1;
			long ___mutationsPointerValue;
			unsigned long ___count;
			while ((___count = enumerateWithState((strings), &___state, ___buffer, 16))) {
				if (!___firstLoop && ___mutationsPointerValue != *___state.mutationsPointer) {
					(__builtin_expect(!(0 && "Object mutated while iterating"), 0) ? __assert_rtn(__func__, "/Volumes/Macintosh HD/Users/averello/Programing/C Programing/cobj/test/testFastEnumeration.c", 21, "0 && \"Object mutated while iterating\"") : (void)0);
				}
				___firstLoop = 0;
				___mutationsPointerValue = *___state.mutationsPointer;
				StringRef string;
				for (unsigned long ___index=0; ___index<___count; ___index++) {
					string = ___state.itemsPointer[___index]; {
						puts(getStringText(string));
					}
				}
			}
		}
		
//		foreach_start(StringRef, string, strings) {
//			 puts(getStringText(string));
//		} foreach_end()
		
		release(strings);
	}
	return EXIT_SUCCESS;
}
