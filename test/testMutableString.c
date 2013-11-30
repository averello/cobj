//
//  testMutableString.c
//  CObjects
//
//  Created by averello on 8/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <errno.h>

extern int errno;

#include <cobj.h>

int main () {
	{ /* Testing allocation */
		MutableStringRef mString = new(MutableString, "string", NULL);
		
		assert( mString != NULL );
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == 6 );
		
		release(mString);
	}
	
	{	/* Testing append */
		
		MutableStringRef mString = new(MutableString, "string", NULL);
		StringRef append = new(String, " 1", NULL);
		appendString(mString, append);
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == 8 );
		assert( strcmp(getStringText(mString), "string 1") == 0 );
		
		release(mString), release(append);
	}
	
	{	/* Testing set */
		
		MutableStringRef mString = new(MutableString, "string", NULL);
		StringRef append = new(String, " 1", NULL);
		
		setString(mString, append);
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == 2 );
		assert( strcmp(getStringText(mString), " 1") == 0 );
		
		release(mString), release(append);
	}
	
	{	/* Testing appendFormat */
		
		MutableStringRef mString = new(MutableString, "string", NULL);
		
		appendFormat(mString, " %d", 1, NULL);
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == 8 );
		assert( strcmp(getStringText(mString), "string 1") == 0 );
		
		release(mString);
	}
	
	{	/* Testing setMutableStringLength */
		
		MutableStringRef mString = new(MutableString, "string 01 02 03 04", NULL);
		
		setMutableStringLength(mString, 6);
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == 6 );
		assert( strcmp(getStringText(mString), "string") == 0 );
		
		release(mString);
	}
	
	{	/* Testing insertStringAtMutableStringIndex */
		
		MutableStringRef mString = new(MutableString, "string text, nice world", NULL);
		StringRef needle = new(String, "big ", NULL);
		size_t length = getStringLength(mString);
		
		insertStringAtMutableStringIndex(mString, needle, 7);
		
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == length + getStringLength(needle) );
		assert( strcmp(getStringText(mString), "string big text, nice world") == 0 );
		
		length = getStringLength(mString);
		insertStringAtMutableStringIndex(mString, needle, length);
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == length + getStringLength(needle) );
		assert( strcmp(getStringText(mString), "string big text, nice worldbig ") == 0 );
		
		release(mString), release(needle);
	}
	
	
	{	/* Testing deleteMutableStringCharactersInRange */
		
		MutableStringRef mString = new(MutableString, "string text, nice world", NULL);
		size_t length = getStringLength(mString);
		
		deleteMutableStringCharactersInRange(mString, SMakeRange(7, 6));
		assert( getStringText(mString) != NULL );
		assert( getStringLength(mString) == (length - 6) );
		assert( strcmp(getStringText(mString), "string nice world") == 0 );
		
		release(mString);
	}
	
	return 0;
}

