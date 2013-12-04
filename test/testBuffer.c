//
//  testBuffer.c
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <unistd.h>

#include <cobj.h>

int main () {
	/* Testing creation */
	{
		char buf[BUFSIZ];
		BufferRef buffer = new(Buffer, buf, 0, NULL);
		assert( buffer == NULL );
		
		buffer = new(Buffer, NULL, 1, NULL);
		assert( buffer == NULL );
		
		buffer = new(Buffer, buf, 1, NULL);
		assert( buffer != NULL );
		release(buffer);
	}
	
	/* Testing getters */
	{
		const char *string = "this is a nice string";
		char buf[BUFSIZ];
		strncpy(buf, string, strlen(string)+1);

		BufferRef buffer = new(Buffer, buf, strlen(buf), NULL);
		const void * bufferBuf = getBufferBytes(buffer);

		assert( strncmp(bufferBuf, buf, strlen(buf)) == 0 );
		assert( getBufferLength(buffer) == strlen(buf) );
		
		char bufferToFill[BUFSIZ];
		memset(bufferToFill, '\0', BUFSIZ);
		getBufferBytesOfLength(buffer, bufferToFill, strlen(buf)/2);
		assert( strncmp(buf, bufferToFill, strlen(buf)/2 ) == 0 );
		
		memset(bufferToFill, '\0', BUFSIZ);
		SRange range = SMakeRange(5, 9);
		getBufferBytesInRange(buffer, bufferToFill, range);
		assert( strncmp(buf+range.location, bufferToFill, range.length ) == 0 );
		
		{
			range = SMakeRange(INT_MAX, 9);
			memset(bufferToFill, '\0', BUFSIZ);
			getBufferBytesInRange(buffer, bufferToFill, range);
			
			char emptyBuffer[BUFSIZ];
			memset(emptyBuffer, '\0', BUFSIZ);
			assert( strcmp(emptyBuffer, bufferToFill) == 0 );
		}
		
		release(buffer);
	}
	return 0;
}
