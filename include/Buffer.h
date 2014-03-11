//
//  Buffer.h
//  CObjects
//
//  Created by averello on 4/3/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_Buffer_h
#define CObjects_Buffer_h

CO_DECLARE_CLASS(Buffer)

const void *getBufferBytes(const void *const self);
void getBufferBytesOfLength(const void *const self, void *restrict buffer, UInteger length);
void getBufferBytesInRange(const void *const self, void *restrict buffer, Range range);
UInteger getBufferLength(const void *const self);

#endif
