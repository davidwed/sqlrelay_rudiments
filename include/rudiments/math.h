// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MATH_H
#define RUDIMENTS_MATH_H

#include <rudiments/private/math.h>

// wrap:
//	stdlib.h - abs(), labs(), llabs(), div(), ldiv(), lldiv()
//	math.h/tgmath.h - fpclassify()
//			signbit()
//			isfinite()
//			isnormal()
//			isnan()
//			isinf()
//			isgreater(),isgreaterequal()
//			isless(),islessequal()
//			islessgreater()
//			isunordered()
//			acos(),asin(),atan(),atan2()
//			cos(),sin(),tan()
//			acosh(),asinh(),atanh(),
//			cosh(),sinh(),tanh()
//			exp(),frexp(),ldexp(),log(),log10()
//			expm1(),log1p(),logb(),exp2(),log2()
//			pow(),sqrt(),hypot(),cbrt()
//			ceil(),fabs(),floor(),fmod()
//			nearbyint(),round(),trunc()
//			remquo()
//			lrint(),llrint()
//			lround(),llround()
//			copysign(),
//			erf(),erfc(),tgamma(),lgamma(),
//			rint()
//			nextafter(),nexttoward(),
//			remainder()
//			scalb(),scalbn(),scalbln()
//			ilogb()
//			fdim(),fmax(),fmin()
//			fma()
//			carg(),conj(),cproj()
//			cimag(),creal()
//	may not be in solaris - 
//		inttypes.h - some integer math functions
//			imaxabs(),imaxdiv()
//			strtoimax(),strtoumax(),wcstoimax(),wcstoumax()
//	not in solaris - 
//		sys/param.h - howmany(),roundup(),powerof2(),MIN(),MAX()

class math {
	public:

	#include <rudiments/private/math.h>
};

#endif
