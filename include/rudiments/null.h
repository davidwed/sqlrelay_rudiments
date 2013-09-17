// Copyright (c) 2013 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_NULL_H
#define RUDIMENTS_NULL_H

// NULL is typically defined in stddef.h
#include <stddef.h>

// Certain versions of gcc define NULL as ((void *)0) and then complain when
// you set a const pointer to it.  Work around that.
#if __GNUC__ <= 2 && __GNUC_MINOR__ <=7
#undef NULL
#define NULL 0
#endif

#endif
