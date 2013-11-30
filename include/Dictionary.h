//
//  Dictionary.h
//  CObjects
//
//  Created by George Boumis on 14/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Dictionary_h
#define CObjects_Dictionary_h

#include <Object.h>

extern const void * Dictionary;
extern const void * DictionaryClass;
typedef void * DictionaryRef;

void initDictionary() __attribute__ ((constructor));
void deallocDictionary() __attribute__ ((destructor));

ArrayRef getKeysCopy(const void *const self);
ArrayRef getValuesCopy(const void *const self);

ObjectRef objectForKey(const void *const self, void *const key);

#endif
