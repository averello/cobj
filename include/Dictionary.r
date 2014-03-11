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


CO_BEGIN_CLASS_TYPE_DECL(Dictionary,Collection)
	ArrayRef keys;
	ArrayRef values;
	UInteger *hashes;
	UInteger count;
CO_END_CLASS_TYPE_DECL

CO_BEGIN_CLASS_DECL(DictionaryClass,CollectionClass)
	ObjectRef ( *objectForKey) (const void *const self, void *const key);
	ArrayRef ( *getKeysCopy) (const void *const self);
	ArrayRef ( *getValuesCopy)(const void *const self);
//	UInteger ( *getCount)(const void *const self);
CO_END_CLASS_DECL



#endif
