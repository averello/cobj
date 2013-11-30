//
//  minunit.h
//  CObjects
//
//  Created by George Boumis on 12/2/13.
//  Copyright (c) 2013 George Boumis. All rights reserved.
//

#ifndef CObjects_minunit_h
#define CObjects_minunit_h

#define mu_assert(message, test) do { if (!(test)) { tests_failed++; return (char *)message; } } while (0)
#define mu_run_test(test) do { char *message = (char *)test(); tests_run++; \
if (message!=NULL) return (char *)message; } while (0)

extern int tests_run;
extern int tests_failed;

#endif
