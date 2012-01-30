// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MATH_H
#define RUDIMENTS_MATH_H

#include <rudiments/private/mathincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class math {
	public:
		static int32_t	absoluteValue(int32_t j);
		static div_t	divide(int32_t numer, int32_t denom);

		static long	absoluteValue(long j);
		static ldiv_t	divide(long numer, long denom);

		static int64_t	absoluteValue(int64_t j);
		static lldiv_t	divide(int64_t numer, int64_t denom);

		static bool	isFinite(float x);
		static bool	isInfinite(float x);

		static bool	isNaN(float x);
		static bool	areNaN(float x, float y);

		static bool	isNormal(float x);
		static bool	isSubNormal(float x);

		static bool	isGreater(float x, float y);
		static bool	isGreaterOrEqual(float x, float y);
		static bool	isLess(float x, float y);
		static bool	isLessOrEqual(float x, float y);
		static bool	isLessOrGreater(float x, float y);

		static bool	isSignBitSet(float x);
		static float	copySignBit(float x, float y);

		static float	arcCosine(float x);
		static float	arcSine(float x);
		static float	arcTangent(float x);
		static float	arcTangent(float y, float x);

		static float	cosine(float x);
		static float	sine(float x);
		static float	tangent(float x);

		static float	hyperbolicArcCosine(float x);
		static float	hyperbolicArcSine(float x);
		static float	hyperbolicArcTangent(float x);

		static float	hyperbolicCosine(float x);
		static float	hyperbolicSine(float x);
		static float	hyperbolicTangent(float x);

		static float	naturalExponent(float x);
		static float	naturalLog(float x);
		static float	naturalExponentMinusOne(float x);
		static float	naturalLogPlusOne(float x);

		static float	exponent(float x);

		static float	normalize(float x, int32_t *exp);

		static float	logBase10(float x);

		static float	exponentBase2(float x);
		static float	logBase2(float x);

		static float	power(float x, float y);

		static float	squareRoot(float x);
		static float	cubeRoot(float x);

		static float	hypotenuse(float x, float y);

		static float	computeExponent(float x);
		static int32_t	integralExponent(float x);
		static float 	loadExponent(float x, int32_t exp);

		static float	ceiling(float x);
		static float	floor(float x);
		static float	absoluteValue(float x);

		static float	remainder(float x, float y);
		static float	remainder(float x, float y, int32_t *quo);

		static float	truncate(float x);
		static float	nearbyInteger(float x);
		static float	round(float x);
		static float	roundInexact(float x);
		static long	roundToLong(float x);
		static int64_t	roundToLongLong(float x);
		static long	roundAwayFromZeroToLong(float x);
		static int64_t	roundAwayFromZeroToLongLong(float x);
		static float	nextAfter(float x, float y);
		static float	nextToward(float x, float y);

		static float	errorFunction(float x);
		static float	complementaryErrorFunction(float x);

		static float	trueGamma(float x);
		static float	naturalLogGamma(float x);

		static float	scaleByRadixToPower(float x, float n);
		static float	scaleByRadixToPower(float x, int32_t n);
		static float	scaleByRadixToPower(float x, long n);

		static float	larger(float x, float y);
		static float	smaller(float x, float y);

		static float	multiplyAndAdd(float x, float y, float z);
		static float	positiveDifference(float x, float y);

		static float		argument(float complex z);
		static float complex	conjugate(float complex z);
		static float complex	project(float complex z);
		static float		imaginary(float complex z);
		static float		real(float complex z);




		// double methods
		static bool	isFinite(double x);
		static bool	isInfinite(double x);

		static bool	isNaN(double x);
		static bool	areNaN(double x, double y);

		static bool	isNormal(double x);
		static bool	isSubNormal(double x);

		static bool	isGreater(double x, double y);
		static bool	isGreaterOrEqual(double x, double y);
		static bool	isLess(double x, double y);
		static bool	isLessOrEqual(double x, double y);
		static bool	isLessOrGreater(double x, double y);

		static bool	isSignBitSet(double x);
		static double	copySignBit(double x, double y);

		static double	arcCosine(double x);
		static double	arcSine(double x);
		static double	arcTangent(double x);
		static double	arcTangent(double y, double x);

		static double	cosine(double x);
		static double	sine(double x);
		static double	tangent(double x);

		static double	hyperbolicArcCosine(double x);
		static double	hyperbolicArcSine(double x);
		static double	hyperbolicArcTangent(double x);

		static double	hyperbolicCosine(double x);
		static double	hyperbolicSine(double x);
		static double	hyperbolicTangent(double x);

		static double	naturalExponent(double x);
		static double	naturalLog(double x);
		static double	naturalExponentMinusOne(double x);
		static double	naturalLogPlusOne(double x);

		static double	exponent(double x);

		static double	normalize(double x, int32_t *exp);

		static double	logBase10(double x);

		static double	exponentBase2(double x);
		static double	logBase2(double x);

		static double	power(double x, double y);

		static double	squareRoot(double x);
		static double	cubeRoot(double x);

		static double	hypotenuse(double x, double y);

		static double	computeExponent(double x);
		static int32_t	integralExponent(double x);
		static double 	loadExponent(double x, int32_t exp);

		static double	ceiling(double x);
		static double	floor(double x);
		static double	absoluteValue(double x);

		static double	remainder(double x, double y);
		static double	remainder(double x, double y, int32_t *quo);

		static double	truncate(double x);
		static double	nearbyInteger(double x);
		static double	round(double x);
		static double	roundInexact(double x);
		static long	roundToLong(double x);
		static int64_t	roundToLongLong(double x);
		static long	roundAwayFromZeroToLong(double x);
		static int64_t	roundAwayFromZeroToLongLong(double x);
		static double	nextAfter(double x, double y);
		static double	nextToward(double x, double y);

		static double	errorFunction(double x);
		static double	complementaryErrorFunction(double x);

		static double	trueGamma(double x);
		static double	naturalLogGamma(double x);

		static double	scaleByRadixToPower(double x, double n);
		static double	scaleByRadixToPower(double x, int32_t n);
		static double	scaleByRadixToPower(double x, long n);

		static double	larger(double x, double y);
		static double	smaller(double x, double y);

		static double	multiplyAndAdd(double x, double y, double z);
		static double	positiveDifference(double x, double y);

		static double		argument(double complex z);
		static double complex	conjugate(double complex z);
		static double complex	project(double complex z);
		static double		imaginary(double complex z);
		static double		real(double complex z);


		// long double methods
		static bool	isFinite(long double x);
		static bool	isInfinite(long double x);

		static bool	isNaN(long double x);
		static bool	areNaN(long double x, long double y);

		static bool	isNormal(long double x);
		static bool	isSubNormal(long double x);

		static bool	isGreater(long double x, long double y);
		static bool	isGreaterOrEqual(long double x, long double y);
		static bool	isLess(long double x, long double y);
		static bool	isLessOrEqual(long double x, long double y);
		static bool	isLessOrGreater(long double x, long double y);

		static bool		isSignBitSet(long double x);
		static long double	copySignBit(long double x,
							long double y);

		static long double	arcCosine(long double x);
		static long double	arcSine(long double x);
		static long double	arcTangent(long double x);
		static long double	arcTangent(long double y,
							long double x);

		static long double	cosine(long double x);
		static long double	sine(long double x);
		static long double	tangent(long double x);

		static long double	hyperbolicArcCosine(long double x);
		static long double	hyperbolicArcSine(long double x);
		static long double	hyperbolicArcTangent(long double x);

		static long double	hyperbolicCosine(long double x);
		static long double	hyperbolicSine(long double x);
		static long double	hyperbolicTangent(long double x);

		static long double	naturalExponent(long double x);
		static long double	naturalLog(long double x);
		static long double	naturalExponentMinusOne(long double x);
		static long double	naturalLogPlusOne(long double x);

		static long double	exponent(long double x);

		static long double	normalize(long double x, int32_t *exp);

		static long double	logBase10(long double x);

		static long double	exponentBase2(long double x);
		static long double	logBase2(long double x);

		static long double	power(long double x, long double y);

		static long double	squareRoot(long double x);
		static long double	cubeRoot(long double x);

		static long double	hypotenuse(long double x,
							long double y);

		static long double	computeExponent(long double x);
		static int32_t		integralExponent(long double x);
		static long double 	loadExponent(long double x,
							int32_t exp);

		static long double	ceiling(long double x);
		static long double	floor(long double x);
		static long double	absoluteValue(long double x);

		static long double	remainder(long double x,
							long double y);
		static long double	remainder(long double x,
							long double y,
							int32_t *quo);

		static long double	truncate(long double x);
		static long double	nearbyInteger(long double x);
		static long double	round(long double x);
		static long double	roundInexact(long double x);
		static long		roundToLong(long double x);
		static int64_t		roundToLongLong(long double x);
		static long		roundAwayFromZeroToLong(long double x);
		static int64_t		roundAwayFromZeroToLongLong(
								long double x);
		static long double	nextAfter(long double x,
							long double y);
		static long double	nextToward(long double x,
							long double y);

		static long double	errorFunction(long double x);
		static long double	complementaryErrorFunction(
								long double x);

		static long double	trueGamma(long double x);
		static long double	naturalLogGamma(long double x);


		static long double	scaleByRadixToPower(long double x,
								long double n);
		static long double	scaleByRadixToPower(long double x,
								int32_t n);
		static long double	scaleByRadixToPower(long double x,
								long n);

		static long double	larger(long double x, long double y);
		static long double	smaller(long double x, long double y);

		static long double	multiplyAndAdd(long double x,
								long double y,
								long double z);
		static long double	positiveDifference(long double x,
								long double y);

		static long double		argument(long double complex z);
		static long double complex	conjugate(long double complex z);
		static long double complex	project(long double complex z);
		static long double		imaginary(long double complex z);
		static long double		real(long double complex z);


//	may not be in solaris - 
//		inttypes.h - some integer math functions
//			imaxabs(),imaxdiv()
//			strtoimax(),strtoumax(),wcstoimax(),wcstoumax()
//	not in solaris - 
//		sys/param.h - howmany(),roundup(),powerof2(),MIN(),MAX()

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/mathinlines.h>
#endif

#endif
