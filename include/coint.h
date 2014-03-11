//
//  coint.h
//  CObjects
//
//  Created by George Boumis on 11/3/2014.
//  Copyright (c) 2014 George Boumis. All rights reserved.
//

#ifndef CObjects_coint_h
#define CObjects_coint_h

#include <stdbool.h>

typedef long Integer;
typedef unsigned long UInteger;

#define IntegerMax    LONG_MAX
#define IntegerMin    LONG_MIN
#define UIntegerMax   ULONG_MAX

#define INTEGER_DEFINED 1

#define NotFound UIntegerMax


#if !defined(YES)
#define YES	(bool)true
#endif

#if !defined(NO)
#define NO	(bool)false
#endif

#if !defined(MIN)
#define MIN(A,B)	((A) < (B) ? (A) : (B))
#endif

#if !defined(MAX)
#define MAX(A,B)	((A) > (B) ? (A) : (B))
#endif


#endif
