//
//  main.c
//  CObjects
//
//  Created by George Boumis on 9/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <cobj.h>
#include <string.h>

#if DEBUG
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#else
#define assert(e)
#endif /* DEBUG */

int main(int argc, const char * argv[])
{
	initString();
	
	void * string1 = new(String, "my precious string", NULL);
	StringRef string2 = new(String, "This is 2", NULL);
	StringRef string3 = new(String, "my precious string", NULL);
	
	assert( equals(string1, string1) );
	assert ( !equals(string1, string2) );
	assert ( equals(string1, string3) );
	
	StringRef string4 = copy(string2);
	
	assert ( equals(string4, string2) );
	
	char fifthChar;
	assert ( characterAtIndex(string1, &fifthChar, 5) != -1 );
	assert(fifthChar == 'e');

	assert( characterAtIndex(string1, &fifthChar, 18) == -1 );
	
	SRange range = SMakeRange(3, 8);
	char buffer[8];
	assert ( getCharactersInRange(string3, buffer, range) != -1 );
	assert( strncmp(buffer, "precious", range.length) == 0 );
	
	StringRef concat = copyStringByAppendingString(string2, string1);
	assert( strcmp(getStringText(concat), "This is 2my precious string") == 0);
	
	assert( getStringLength(string1) + getStringLength(string2) == getStringLength(concat) );
	
	StringRef bigString = new(String, "my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string my precious string ", NULL);
	
	StringRef formatedString = newStringWithFormat(String, "%d %s %s", 0, getStringText(string1), getStringText(bigString));
	
	{
		StringRef l1 = new(String, NULL);
		StringRef l2 = new(String, "Hello world");
		
		assert ( ! equals(l1, l2) );
		
		release(l1);
		release(l2);
	}

	{
		StringRef k1 = new(String, "Georges", NULL);
		StringRef k2 = new(String, "Romain", NULL);
		assert( compare(k1, k2) == SAscending );
		assert( compare(k1, k1) == SSame );
		assert( compare(k2, k1) == SDescending );
		release(k1);
		release(k2);
	}
	
	{
		unsigned long h1 = hash(string1);
		unsigned long h2 = hash(string2);
		unsigned long h3 = hash(string3);
		unsigned long h4 = hash(string4);
//		printf("%lu\n", sizeof(unsigned long));
//		printf("%lu %lu %lu\n", h1, h1 >> (sizeof(unsigned long) * 8 / 2) , (h1 << (sizeof(unsigned long) * 8 / 2)) >> (sizeof(unsigned long) * 8 /2));
//		printf("%lu %lu %lu\n", h2, h2 >> (sizeof(unsigned long) * 8 / 2) , (h2 << (sizeof(unsigned long) * 8 / 2)) >> (sizeof(unsigned long) * 8 /2));
		assert( h1 == h3 );
		assert( h2 == h4 );
	}
	
	{
		const char *string = "   nb  ia\r \t\r\n ";
		StringRef whiteString = new(String, string, NULL);
		StringRef trimmedString = copyStringByTrimmingSpaces(whiteString);
		assert( trimmedString != NULL );
		assert( getStringText(trimmedString) != NULL );
		assert( strcmp(getStringText(trimmedString), "nb  ia") == 0 );
		assert( strcmp(getStringText(whiteString), string) == 0 );
		release(trimmedString);
		release(whiteString);
	}

	release(bigString);
	release(formatedString);
	release(concat);
	release(string1);
	release(string2);
	release(string3);
	release(string4);
	//*/
	
	deallocString();
	return EXIT_SUCCESS;
}

