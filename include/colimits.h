//
//  colimits.c
//  CObjects
//
//  Created by averello on 11/03/2014.
//  Copyright (c) 2014 George Boumis. All rights reserved.
//


#ifndef CObjects_colimits_h
#define CObjects_colimits_h

#include <stdint.h>
#include <limits.h>

#ifndef INT8_MIN
#define INT8_MIN (-INT8_MAX - 1)
#endif
#ifndef INT8_MAX
#define INT8_MAX 7
#endif

#ifndef INT16_MIN
#define INT16_MIN (-INT16_MAX - 1)
#endif
#ifndef INT16_MAX
#define INT16_MAX 127
#endif

#ifndef INT32_MIN
#define INT32_MIN (-INT32_MAX - 1)
#endif
#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif

#ifndef INT64_MIN
#define INT64_MIN (-INT64_MAX - 1)
#endif
#ifndef INT64_MAX
#define INT64_MAX 9223372036854775807L
#endif


#ifndef UINT8_MIN
#define UINT8_MIN 0
#endif
#ifndef UINT8_MAX
#define UINT8_MAX 15
#endif

#ifndef UINT16_MIN
#define UINT16_MIN 0
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 255
#endif

#ifndef UINT32_MIN
#define UINT32_MIN 0
#endif
#ifndef UINT32_MAX
#define UINT32_MAX 4294967295
#endif

#ifndef UINT64_MIN
#define UINT64_MIN 0
#endif
#ifndef UINT64_MAX
#define UINT64_MAX 18446744073709551615UL
#endif

#endif /* CObjects_colimits_h */
