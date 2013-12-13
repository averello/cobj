//
//  Dictionary.r
//  CObjects
//
//  Created by George Boumis on 14/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Dictionary_r
#define CObjects_Dictionary_r

#include <cobj.h>
#include <Object.r>
#include <Collection.h>
#include <Collection.r>

struct Dictionary {
	const struct Collection isa;
	ArrayRef keys;
	ArrayRef values;
	unsigned long *hashes;
	unsigned long count;
};

struct DictionaryClass {
	const struct CollectionClass isa;
	ObjectRef ( *objectForKey) (const void *const self, void *const key);
	ArrayRef ( *getKeysCopy) (const void *const self);
	ArrayRef ( *getValuesCopy)(const void *const self);
//	unsigned long ( *getCount)(const void *const self);
};



#endif
