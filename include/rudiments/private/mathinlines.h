RUDIMENTS_INLINE int math::absoluteValue(int j) {
	return abs(j);
}

RUDIMENTS_INLINE div_t math::divide(int numer, int denom) {
	return div(numer,denom);
}

RUDIMENTS_INLINE long math::absoluteValue(long j) {
	return labs(j);
}

RUDIMENTS_INLINE ldiv_t math::divide(long numer, long denom) {
	return ldiv(numer,denom);
}

RUDIMENTS_INLINE long long math::absoluteValue(long long j) {
	return llabs(j);
}

RUDIMENTS_INLINE lldiv_t math::divide(long long numer, long long denom) {
	return lldiv(numer,denom);
}




// float methods

RUDIMENTS_INLINE bool math::isFinite(float x) {
	return isfinite(x);
}

RUDIMENTS_INLINE bool math::isNormal(float x) {
	return isnormal(x);
}

RUDIMENTS_INLINE bool math::isSubNormal(float x) {
	return (fpclassify()==FP_SUBNORMAL);
}

RUDIMENTS_INLINE bool math::isNaN(float x) {
	return isnan(x);
}

RUDIMENTS_INLINE bool math::isInfinite(float x) {
	return isinf(x);
}

RUDIMENTS_INLINE bool math::isGreater(float x, float y) {
	return isgreater(x,y);
}

RUDIMENTS_INLINE bool math::isGreaterOrEqual(float x, float y) {
	return isgreaterequal(x,y);
}

RUDIMENTS_INLINE bool math::isLess(float x, float y) {
	return isless(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrEqual(float x, float y) {
	return islessequal(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrGreater(float x, float y) {
	return islessgreater(x,y);
}

RUDIMENTS_INLINE bool math::areNaN(float x, float y) {
	return isunordered(x,y);
}

RUDIMENTS_INLINE bool math::isSignBitSet(float x) {
	return signbit(x);
}

RUDIMENTS_INLINE float math::arcCosine(float x) {
	return acosf(x);
}

RUDIMENTS_INLINE float math::arcSine(float x) {
	return asinf(x);
}

RUDIMENTS_INLINE float math::arcTangent(float x) {
	return atanf(x);
}

RUDIMENTS_INLINE float math::arcTangent(float y, float x) {
	return atan2f(y,x);
}

RUDIMENTS_INLINE float math::cosine(float x) {
	return cosf(x);
}

RUDIMENTS_INLINE float math::sine(float x) {
	return sinf(x);
}

RUDIMENTS_INLINE float math::tangent(float x) {
	return tanf(x);
}

RUDIMENTS_INLINE float math::hyperbolicArcCosine(float x) {
	return acoshf(x);
}

RUDIMENTS_INLINE float math::hyperbolicArcSine(float x) {
	return asinhf(x);
}

RUDIMENTS_INLINE float math::hyperbolicArcTangent(float x) {
	return atanhf(x);
}

RUDIMENTS_INLINE float math::hyperbolicCosine(float x) {
	return coshf(x);
}

RUDIMENTS_INLINE float math::hyperbolicSine(float x) {
	return sinhf(x);
}

RUDIMENTS_INLINE float math::hyperbolicTangent(float x) {
	return tanhf(x);
}

RUDIMENTS_INLINE float math::naturalExponent(float x) {
	return expf(x);
}

RUDIMENTS_INLINE float math::normalize(float x, int *exp) {
	return frexpf(x,exp);
}

RUDIMENTS_INLINE float math::naturalLog(float x) {
	return logf(x);
}

RUDIMENTS_INLINE float math::logBase10(float x) {
	return log10f(x);
}

RUDIMENTS_INLINE float math::naturalExponentMinusOne(float x) {
	return expm1f(x);
}

RUDIMENTS_INLINE float math::naturalLogPlusOne(float x) {
	return log1pf(x);
}

RUDIMENTS_INLINE float math::logbf(float x) {
	return logbf(x);
}

RUDIMENTS_INLINE float math::exponentBase2(float x) {
	return exp2f(x);
}

RUDIMENTS_INLINE float math::logBase2(float x) {
	return log2f(x);
}

RUDIMENTS_INLINE float math::power(float x, float y) {
	return powf(x,y);
}

RUDIMENTS_INLINE float math::squareRoot(float x) {
	return sqrt(x);
}

RUDIMENTS_INLINE float math::hypotenuse(float x, float y) {
	return hypotf(x,y);
}

RUDIMENTS_INLINE float math::cubeRoot(float x) {
	return cbrtf(x);
}

RUDIMENTS_INLINE float math::ceiling(float x) {
	return ceilf(x);
}

RUDIMENTS_INLINE float math::absoluteValue(float x) {
	return fabsf(x);
}

RUDIMENTS_INLINE float math::floor(float x) {
	return floorf(x);
}

RUDIMENTS_INLINE float math::remainder(float x, float y) {
	return fmodf(x,y);
}

RUDIMENTS_INLINE float math::nearbyInteger(float x) {
	return nearbyintf(x);
}

RUDIMENTS_INLINE float math::round(float x) {
	return roundf(x);
}

RUDIMENTS_INLINE float math::truncate(float x) {
	return truncf(x);
}

RUDIMENTS_INLINE float math::remainder(float x, float y, int *quo) {
	return remquof(x,y,quo);
}

RUDIMENTS_INLINE long math::roundToLong(float x) {
	return lrintf(x);
}

RUDIMENTS_INLINE long long math::roundToLongLong(float x) {
	return llrintf(x);
}

RUDIMENTS_INLINE long math::roundAwayFromZeroToLong(float x) {
	return lroundf(x);
}

RUDIMENTS_INLINE long long math::roundAwayFromZeroToLongLong(float x) {
	return llroundf(x);
}

RUDIMENTS_INLINE float math::copySignBit(float x, float y) {
	return copysignf(x,y);
}

RUDIMENTS_INLINE float math::errorFunction(float x) {
	return erff(x);
}

RUDIMENTS_INLINE float math::complementaryErrorFunction(float x) {
	return erfcf(x);
}

RUDIMENTS_INLINE float math::trueGamma(float x) {
	return tgammaf(x);
}

RUDIMENTS_INLINE float math::naturalLogGamma(float x) {
	return lgammaf(x);
}

RUDIMENTS_INLINE float math::roundInexact(float x) {
	return rintf(x);
}

RUDIMENTS_INLINE float math::nextAfter(float x, float y) {
	return nextafterf(x,y);
}

RUDIMENTS_INLINE float math::nextToward(float x, float y) {
	return nexttowardf(x,y);
}

RUDIMENTS_INLINE float math::scaleByRadixToPower(float x, float n) {
	return scalbf(x,n) {
}

RUDIMENTS_INLINE float math::scaleByRadixToPower(float x, int n) {
	return scalbnf(x,n) {
}

RUDIMENTS_INLINE float math::scaleByRadixToPower(float x, long n) {
	return scalblnf(x,n) {
}

RUDIMENTS_INLINE int math::integralExponent(float x) {
	return ilogbf(x);
}

RUDIMENTS_INLINE float math::positiveDifference(float x, float y) {
	return fdimf(x,y);
}

RUDIMENTS_INLINE float math::larger(float x, float y) {
	return fmaxf(x,y);
}

RUDIMENTS_INLINE float math::smaller(float x, float y) {
	return fminf(x,y);
}

RUDIMENTS_INLINE float math::multiplyAndAdd(float x, float y, float z) {
	return fmaf(x,y,z);
}

RUDIMENTS_INLINE float math::argument(float complex z) {
	return cargf(z);
}

RUDIMENTS_INLINE float math::conjugate(float complex z) {
	return conjf(z);
}

RUDIMENTS_INLINE float math::project(float complex z) {
	return cprojf(z);
}

RUDIMENTS_INLINE float math::imaginary(float complex z) {
	return cimagf(z);
}

RUDIMENTS_INLINE float math::real(float complex z) {
	return crealf(z);
}



// double methods

RUDIMENTS_INLINE bool math::isFinite(double x) {
	return isfinite(x);
}

RUDIMENTS_INLINE bool math::isNormal(double x) {
	return isnormal(x);
}

RUDIMENTS_INLINE bool math::isSubNormal(double x) {
	return (fpclassify()==FP_SUBNORMAL);
}

RUDIMENTS_INLINE bool math::isNaN(double x) {
	return isnan(x);
}

RUDIMENTS_INLINE bool math::isInfinite(double x) {
	return isin(x);
}

RUDIMENTS_INLINE bool math::isGreater(double x, double y) {
	return isgreater(x,y);
}

RUDIMENTS_INLINE bool math::isGreaterOrEqual(double x, double y) {
	return isgreaterequal(x,y);
}

RUDIMENTS_INLINE bool math::isLess(double x, double y) {
	return isless(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrEqual(double x, double y) {
	return islessequal(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrGreater(double x, double y) {
	return islessgreater(x,y);
}

RUDIMENTS_INLINE bool math::areNaN(double x, double y) {
	return isunordered(x,y);
}

RUDIMENTS_INLINE bool math::isSignBitSet(double x) {
	return signbit(x);
}

RUDIMENTS_INLINE double math::arcCosine(double x) {
	return acos(x);
}

RUDIMENTS_INLINE double math::arcSine(double x) {
	return asin(x);
}

RUDIMENTS_INLINE double math::arcTangent(double x) {
	return atan(x);
}

RUDIMENTS_INLINE double math::arcTangent(double y, double x) {
	return atan2(y,x);
}

RUDIMENTS_INLINE double math::cosine(double x) {
	return cos(x);
}

RUDIMENTS_INLINE double math::sine(double x) {
	return sin(x);
}

RUDIMENTS_INLINE double math::tangent(double x) {
	return tan(x);
}

RUDIMENTS_INLINE double math::hyperbolicArcCosine(double x) {
	return acosh(x);
}

RUDIMENTS_INLINE double math::hyperbolicArcSine(double x) {
	return asinh(x);
}

RUDIMENTS_INLINE double math::hyperbolicArcTangent(double x) {
	return atanh(x);
}

RUDIMENTS_INLINE double math::hyperbolicCosine(double x) {
	return cosh(x);
}

RUDIMENTS_INLINE double math::hyperbolicSine(double x) {
	return sinh(x);
}

RUDIMENTS_INLINE double math::hyperbolicTangent(double x) {
	return tanh(x);
}

RUDIMENTS_INLINE double math::naturalExponent(double x) {
	return exp(x);
}

RUDIMENTS_INLINE double math::normalize(double x, int *exp) {
	return frexp(x,exp);
}

RUDIMENTS_INLINE double math::naturalLog(double x) {
	return log(x);
}

RUDIMENTS_INLINE double math::logBase10(double x) {
	return log10(x);
}

RUDIMENTS_INLINE double math::naturalExponentMinusOne(double x) {
	return expm1(x);
}

RUDIMENTS_INLINE double math::naturalLogPlusOne(double x) {
	return log1p(x);
}

RUDIMENTS_INLINE double math::logb(double x) {
	return logb(x);
}

RUDIMENTS_INLINE double math::exponentBase2(double x) {
	return exp2(x);
}

RUDIMENTS_INLINE double math::logBase2(double x) {
	return log2(x);
}

RUDIMENTS_INLINE double math::power(double x, double y) {
	return pow(x,y);
}

RUDIMENTS_INLINE double math::squareRoot(double x) {
	return sqrt(x);
}

RUDIMENTS_INLINE double math::hypotenuse(double x, double y) {
	return hypot(x,y);
}

RUDIMENTS_INLINE double math::cubeRoot(double x) {
	return cbrt(x);
}

RUDIMENTS_INLINE double math::ceiling(double x) {
	return ceil(x);
}

RUDIMENTS_INLINE double math::absoluteValue(double x) {
	return fabs(x);
}

RUDIMENTS_INLINE double math::floor(double x) {
	return floor(x);
}

RUDIMENTS_INLINE double math::remainder(double x, double y) {
	return fmod(x,y);
}

RUDIMENTS_INLINE double math::nearbyInteger(double x) {
	return nearbyint(x);
}

RUDIMENTS_INLINE double math::round(double x) {
	return round(x);
}

RUDIMENTS_INLINE double math::truncate(double x) {
	return trunc(x);
}

RUDIMENTS_INLINE double math::remainder(double x, double y, int *quo) {
	return remquo(x,y,quo);
}

RUDIMENTS_INLINE long math::roundToLong(double x) {
	return lrint(x);
}

RUDIMENTS_INLINE long long math::roundToLongLong(double x) {
	return llrint(x);
}

RUDIMENTS_INLINE long math::roundAwayFromZeroToLong(double x) {
	return lround(x);
}

RUDIMENTS_INLINE long long math::roundAwayFromZeroToLongLong(double x) {
	return llround(x);
}

RUDIMENTS_INLINE double math::copySignBit(double x, double y) {
	return copysign(x,y);
}

RUDIMENTS_INLINE double math::errorFunction(double x) {
	return erf(x);
}

RUDIMENTS_INLINE double math::complementaryErrorFunction(double x) {
	return erfc(x);
}

RUDIMENTS_INLINE double math::trueGamma(double x) {
	return tgamma(x);
}

RUDIMENTS_INLINE double math::naturalLogGamma(double x) {
	return lgamma(x);
}

RUDIMENTS_INLINE double math::roundInexact(double x) {
	return rint(x);
}

RUDIMENTS_INLINE double math::nextAfter(double x, double y) {
	return nextafter(x,y);
}

RUDIMENTS_INLINE double math::nextToward(double x, double y) {
	return nexttoward(x,y);
}

RUDIMENTS_INLINE double math::scaleByRadixToPower(double x, double n) {
	return scalb(x,n) {
}

RUDIMENTS_INLINE double math::scaleByRadixToPower(double x, int n) {
	return scalbn(x,n) {
}

RUDIMENTS_INLINE double math::scaleByRadixToPower(double x, long n) {
	return scalbln(x,n) {
}

RUDIMENTS_INLINE int math::integralExponent(double x) {
	return ilogb(x);
}

RUDIMENTS_INLINE double math::positiveDifference(double x, double y) {
	return fdim(x,y);
}

RUDIMENTS_INLINE double math::larger(double x, double y) {
	return fmax(x,y);
}

RUDIMENTS_INLINE double math::smaller(double x, double y) {
	return fmin(x,y);
}

RUDIMENTS_INLINE double math::multiplyAndAdd(double x, double y, double z) {
	return fma(x,y,z);
}

RUDIMENTS_INLINE double math::argument(double complex z) {
	return carg(z);
}

RUDIMENTS_INLINE double math::conjugate(double complex z) {
	return conj(z);
}

RUDIMENTS_INLINE double math::project(double complex z) {
	return cproj(z);
}

RUDIMENTS_INLINE double math::imaginary(double complex z) {
	return cimag(z);
}

RUDIMENTS_INLINE double math::real(double complex z) {
	return creal(z);
}


// long long double methods

RUDIMENTS_INLINE bool math::isFinite(long double x) {
	return isfinite(x);
}

RUDIMENTS_INLINE bool math::isNormal(long double x) {
	return isnormal(x);
}

RUDIMENTS_INLINE bool math::isSubNormal(long double x) {
	return (fpclassify()==FP_SUBNORMAL);
}

RUDIMENTS_INLINE bool math::isNaN(long double x) {
	return isnan(x);
}

RUDIMENTS_INLINE bool math::isInfinite(long double x) {
	return isin(x);
}

RUDIMENTS_INLINE bool math::isGreater(long double x, long double y) {
	return isgreater(x,y);
}

RUDIMENTS_INLINE bool math::isGreaterOrEqual(long double x, long double y) {
	return isgreaterequal(x,y);
}

RUDIMENTS_INLINE bool math::isLess(long double x, long double y) {
	return isless(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrEqual(long double x, long double y) {
	return islessequal(x,y);
}

RUDIMENTS_INLINE bool math::isLessOrGreater(long double x, long double y) {
	return islessgreater(x,y);
}

RUDIMENTS_INLINE bool math::areNaN(long double x, long double y) {
	return isunordered(x,y);
}

RUDIMENTS_INLINE bool math::isSignBitSet(long double x) {
	return signbit(x);
}

RUDIMENTS_INLINE long double math::arcCosine(long double x) {
	return acosl(x);
}

RUDIMENTS_INLINE long double math::arcSine(long double x) {
	return asinl(x);
}

RUDIMENTS_INLINE long double math::arcTangent(long double x) {
	return atanl(x);
}

RUDIMENTS_INLINE long double math::arcTangent(long double y, long double x) {
	return atan2l(y,x);
}

RUDIMENTS_INLINE long double math::cosine(long double x) {
	return cosl(x);
}

RUDIMENTS_INLINE long double math::sine(long double x) {
	return sinl(x);
}

RUDIMENTS_INLINE long double math::tangent(long double x) {
	return tanl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicArcCosine(long double x) {
	return acoshl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicArcSine(long double x) {
	return asinhl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicArcTangent(long double x) {
	return atanhl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicCosine(long double x) {
	return coshl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicSine(long double x) {
	return sinhl(x);
}

RUDIMENTS_INLINE long double math::hyperbolicTangent(long double x) {
	return tanhl(x);
}

RUDIMENTS_INLINE long double math::naturalExponent(long double x) {
	return expl(x);
}

RUDIMENTS_INLINE long double math::normalize(long double x, int *exp) {
	return frexpl(x,exp);
}

RUDIMENTS_INLINE long double math::naturalLog(long double x) {
	return logl(x);
}

RUDIMENTS_INLINE long double math::logBase10(long double x) {
	return log10l(x);
}

RUDIMENTS_INLINE long double math::naturalExponentMinusOne(long double x) {
	return expm1l(x);
}

RUDIMENTS_INLINE long double math::naturalLogPlusOne(long double x) {
	return log1pl(x);
}

RUDIMENTS_INLINE long double math::logb(long double x) {
	return logbl(x);
}

RUDIMENTS_INLINE long double math::exponentBase2(long double x) {
	return exp2l(x);
}

RUDIMENTS_INLINE long double math::logBase2(long double x) {
	return log2l(x);
}

RUDIMENTS_INLINE long double math::power(long double x, long double y) {
	return powl(x,y);
}

RUDIMENTS_INLINE long double math::squareRoot(long double x) {
	return sqrtl(x);
}

RUDIMENTS_INLINE long double math::hypotenuse(long double x, long double y) {
	return hypotl(x,y);
}

RUDIMENTS_INLINE long double math::cubeRoot(long double x) {
	return cbrtl(x);
}

RUDIMENTS_INLINE long double math::ceiling(long double x) {
	return ceill(x);
}

RUDIMENTS_INLINE long double math::absoluteValue(long double x) {
	return fabsl(x);
}

RUDIMENTS_INLINE long double math::floor(long double x) {
	return floorl(x);
}

RUDIMENTS_INLINE long double math::remainder(long double x, long double y) {
	return fmodl(x,y);
}

RUDIMENTS_INLINE long double math::nearbyInteger(long double x) {
	return nearbyintl(x);
}

RUDIMENTS_INLINE long double math::round(long double x) {
	return roundl(x);
}

RUDIMENTS_INLINE long double math::truncate(long double x) {
	return truncl(x);
}

RUDIMENTS_INLINE long double math::remainder(long double x,
						long double y, int *quo) {
	return remquol(x,y,quo);
}

RUDIMENTS_INLINE long math::roundToLong(long double x) {
	return lrintl(x);
}

RUDIMENTS_INLINE long long math::roundToLongLong(long double x) {
	return llrintl(x);
}

RUDIMENTS_INLINE long math::roundAwayFromZeroToLong(long double x) {
	return lroundl(x);
}

RUDIMENTS_INLINE long long math::roundAwayFromZeroToLongLong(long double x) {
	return llroundl(x);
}

RUDIMENTS_INLINE long double math::copySignBit(long double x, long double y) {
	return copysignl(x,y);
}

RUDIMENTS_INLINE long double math::errorFunction(long double x) {
	return erfl(x);
}

RUDIMENTS_INLINE long double math::complementaryErrorFunction(long double x) {
	return erfcl(x);
}

RUDIMENTS_INLINE long double math::trueGamma(long double x) {
	return tgammal(x);
}

RUDIMENTS_INLINE long double math::naturalLogGamma(long double x) {
	return lgammal(x);
}

RUDIMENTS_INLINE long double math::roundInexact(long double x) {
	return rintl(x);
}

RUDIMENTS_INLINE long double math::nextAfter(long double x, long double y) {
	return nextafterl(x,y);
}

RUDIMENTS_INLINE long double math::nextToward(long double x, long double y) {
	return nexttowardl(x,y);
}

RUDIMENTS_INLINE long double math::scaleByRadixToPower(long double x,
							long double n) {
	return scalbl(x,n) {
}

RUDIMENTS_INLINE long double math::scaleByRadixToPower(long double x, int n) {
	return scalbnl(x,n) {
}

RUDIMENTS_INLINE long double math::scaleByRadixToPower(long double x, long n) {
	return scalblnl(x,n) {
}

RUDIMENTS_INLINE int math::integralExponent(long double x) {
	return ilogbl(x);
}

RUDIMENTS_INLINE long double math::positiveDifference(long double x,
							long double y) {
	return fdiml(x,y);
}

RUDIMENTS_INLINE long double math::larger(long double x, long double y) {
	return fmaxl(x,y);
}

RUDIMENTS_INLINE long double math::smaller(long double x, long double y) {
	return fminl(x,y);
}

RUDIMENTS_INLINE long double math::multiplyAndAdd(long double x,
						long double y, long double z) {
	return fmal(x,y,z);
}

RUDIMENTS_INLINE long double math::argument(long double complex z) {
	return cargl(z);
}

RUDIMENTS_INLINE long double math::conjugate(long double complex z) {
	return conjl(z);
}

RUDIMENTS_INLINE long double math::project(long double complex z) {
	return cprojl(z);
}

RUDIMENTS_INLINE long double math::imaginary(long double complex z) {
	return cimagl(z);
}

RUDIMENTS_INLINE long double math::real(long double complex z) {
	return creall(z);
}

RUDIMENTS_INLINE float math::loadExponent(float x, int exp) {
	return ldexpf(x,exp);
}

RUDIMENTS_INLINE double math::loadExponent(double x, int exp) {
	return ldexp(x,exp);
}

RUDIMENTS_INLINE long double math::loadExponent(long double x, int exp) {
	return ldexpl(x,exp);
}
