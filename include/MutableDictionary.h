//
//  MutableDictionary.h
//  CObjects
//
//  Created by George Boumis on 15/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_MutableDictionary_h
#define CObjects_MutableDictionary_h

CO_DECLARE_CLASS(MutableDictionary)

void setObjectForKey(void *const self, void *const object, void *const key);
void setMutableDictionaryLoadFactor(void *const self, float loadFactor);

void removeObjectForKey(void *const self, void *const key);

#ifdef __PROFILING__
void MutableDictionaryPrintfStatistics(const void *const self);
#endif

#endif
