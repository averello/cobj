//
//  testWMutableString.c
//  CObjects
//
//  Created by George Boumis on 30/11/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#include <stdio.h>
#include <cobj.h>
#if DEBUG
#include <assert.h>
#else
#define assert(e)
#endif /* DEBUG */
#include <wchar.h>
#include <locale.h>

int main () {
	fwide(stdout, 1);
	wchar_t w1wcs[] = L"𐍆五色沼 (ごしきぬま) και όλα καλά, my fîrst wïdé strìng磐梯 !";
	WMutableStringRef w1 = new(WMutableString, w1wcs, NULL);
	
	assert( w1 != NULL );
	assert( getWText(w1) != NULL );
	assert( wcscmp(getWText(w1), w1wcs) == 0 );
	WStringRef w2 = newStringWithFormat(WString, L"hello και καλά, %ls", getStringText(w1), NULL);
	assert( getWText(w2) != NULL );
	assert( wcscmp(getWText(w2), L"hello και καλά, 𐍆五色沼 (ごしきぬま) και όλα καλά, my fîrst wïdé strìng磐梯 !") == 0 );
	{
		wchar_t buffer;
		for (UInteger i=0; i<getStringLength(w1); i++) {
			characterAtIndex(w1, &buffer, i);
			assert( w1wcs[i] == buffer );
		}
	}
	{
		wchar_t buffer[9];
		wmemset(buffer, L'\0', 9);
		getCharactersInRange(w2, buffer, MakeRange(6, 8));
		assert( wcscmp(buffer, L"και καλά") == 0 );
	}
	
	{
		WStringRef Yiorgos = new(WString, L"Γιώργος", NULL);
		WStringRef yiorgos = new(WString, L"γιώργος", NULL);
		WStringRef yiannis = new(WString, L"Γιάννης", NULL);
		assert( compare(Yiorgos, yiannis) == SDescending );
		assert( compare(yiannis, Yiorgos) == SAscending );
		//assert( compareWithOptions(yiorgos, Yiorgos, SStringComparingOptionCaseInsensitiveSearch) == SSame );
		//assert( compareWithOptions(yiorgos, Yiorgos, SStringComparingOptionLocalizedCompare) == SDescending );
		
		WStringRef YiorgosCopy = copy(Yiorgos);
		assert (equals(YiorgosCopy, Yiorgos) );
		release(YiorgosCopy);
		release(yiannis);
		release(Yiorgos);
		release(yiorgos);
	}
	
	{
		UInteger h1 = hash(w1);
		UInteger h2 = hash(w2);
		WStringRef w1Copy = copy(w1);
		WStringRef w2Copy = copy(w2);
		UInteger h1copy = hash(w1Copy);
		UInteger h2copy = hash(w2Copy);
		assert( h1copy == h1 );
		assert( h2copy == h2 );
		release(w1Copy);
		release(w2Copy);
	}
	
	release(w1);
	release(w2);
	return 0;
}
