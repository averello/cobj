//
//  testDictionary.c
//  CObjects
//
//  Created by George Boumis on 14/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <cobj.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */

int main () {
	initDictionary();
	initString();
	{
		const char *sk1 = "Hello", *sv1 = "World", *sk2 = "Hello World", *sv2 = "Goodbye World";
		StringRef k1 = new(String, sk1);
		StringRef v1 = new(String, sv1);
		StringRef k2 = new(String, sk2);
		StringRef v2 = new(String, sv2);
		DictionaryRef dico = new(Dictionary, k1, v1, k2, v2, NULL);
		
		release(k1);
		release(v1);
		release(k2);
		release(v2);
		{
			StringRef res1 = objectForKey(dico, k1);
			StringRef res2 = objectForKey(dico, k2);
			assert(  res1 != NULL );
			assert( res2 != NULL );
			assert( objectForKey(dico, v1) == NULL );
			assert( objectForKey(dico, v2) == NULL );
			
			assert( equals(res1, v1) );
			assert( equals(res2, v2) );
		}
		{
			ArrayRef keys = new(Array, k1, k2, NULL);
			ArrayRef values = new(Array, v1, v2, NULL);
			
			ArrayRef copyKeys = getKeysCopy(dico);
			ArrayRef copyValues = getValuesCopy(dico);
			
			assert( equals(copyKeys, keys) );
			assert( equals(copyValues, values) );
			
			assert( arrayContainsObject(copyKeys, k1) );
			assert( arrayContainsObject(copyKeys, k2) );
			
			release(copyValues);
			release(copyKeys);
			
			release(keys);
			release(values);
		}

		release(dico);
	}
	
	deallocString();
	deallocDictionary();
	return 0;
}

