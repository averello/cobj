//
//  testCouple.c
//  CObjects
//
//  Created by averello on 3/3/13.
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
	initCouple();
	initString();
	
	ObjectRef k1, k2, k3;
	ObjectRef v1, v2, v3;
	k1 = new(String, "object 1", NULL);
	k2 = new(String, "object 2", NULL);
	k3 = new(String, "object 3", NULL);
	
	v1 = new(String, "value 1", NULL);
	v2 = new(String, "value 2", NULL);
	v3 = new(String, "value 3", NULL);
	
	/* Test creation and getters */
	{
		CoupleRef couple = new(Couple, k1, v1, NULL);
		
		assert( couple != NULL );
		assert( getKey(couple) == k1 );
		assert( getValue(couple) == v1 );
		
		release(couple);
	}

	/* Test equals */
	{
		CoupleRef couple1 = new(Couple, k1, v1, NULL);
		CoupleRef couple2 = new(Couple, k1, v1, NULL);
		CoupleRef couple3 = new(Couple, k3, v3, NULL);
		
		assert( equals(couple1, couple2) );
		assert( !equals(couple1, couple3) );
		
		release(couple1);
		release(couple2);
		release(couple3);
	}

	/* Test copy */
	{
		CoupleRef couple = new(Couple, k1, v1, NULL);
		CoupleRef copyCouple = copy(couple);
		assert( copyCouple != NULL );
		assert( equals(copyCouple, couple) );
		release(couple);
		release(copyCouple);
	}
	
	/* Test setters */
	{
		CoupleRef couple = new(Couple, k1, v1, NULL);
		
		setValue(couple, k2);
		assert( getValue(couple) == k2 );
		
		setValue(couple, k3);
		assert( getValue(couple) == k3 );
		
		setKey(couple, k2);
		assert( getKey(couple) == k2 );
		
		setKey(couple, k3);
		assert( getKey(couple) == k3 );
		
		release(couple);
	}
	
	release(k1), release(k2), release(k3);
	release(v1), release(v2), release(v3);
	
	deallocString();
	deallocCouple();
	return 0;
}
