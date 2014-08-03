// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/randomnumber.h>
#include <rudiments/device.h>
#include <rudiments/datetime.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_RANDOM_R) && \
	!defined(RUDIMENTS_HAVE_RAND_R) && \
	!defined(RUDIMENTS_HAVE_LRAND48_R)
static threadmutex	*_rnmutex;
#endif

class randomnumberprivate {
	friend class randomnumber;
	private:
		#if defined(RUDIMENTS_HAVE_RANDOM_R)
			char		statebuf[64];
			random_data	buffer;
		#elif defined(RUDIMENTS_HAVE_RAND_R)
			uint32_t	useed;
		#elif defined(RUDIMENTS_HAVE_LRAND48_R)
			drand48_data	buffer;
		#endif
};

randomnumber::randomnumber() {
	pvt=new randomnumberprivate;
}

randomnumber::~randomnumber() {
	delete pvt;
}

bool randomnumber::setSeed(int32_t seed) {

	#if defined(RUDIMENTS_HAVE_ARC4RANDOM_UNIFORM)
		// do nothing, arc4random_uniform is seeded automatically
		return true;
	#elif defined(RUDIMENTS_HAVE_ARC4RANDOM)
		// do nothing, arc4random is seeded automatically
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM_R)
		bytestring::zero(pvt->statebuf,sizeof(pvt->statebuf));
		bytestring::zero(&pvt->buffer,sizeof(pvt->buffer));
		return !initstate_r((uint32_t)seed,
					pvt->statebuf,sizeof(pvt->statebuf),
					&pvt->buffer);
	#elif defined(RUDIMENTS_HAVE_RAND_R)
		pvt->useed=seed;
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48_R)
		bytestring::zero(&pvt->buffer,sizeof(pvt->buffer));
		return !srand48_r(seed,&pvt->buffer);
	#elif defined(RUDIMENTS_HAVE_RANDOM)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srandom(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_RAND)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srand(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srand48(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#else
		#error "Couldn't find a suitable replacement for rand/srand"
	#endif
}

bool randomnumber::generateNumber(int32_t *result) {

	#if defined(RUDIMENTS_HAVE_ARC4RANDOM_UNIFORM)
		*result=arc4random_uniform(getRandMax()/2)+getRandMax()/2;
		return true;
	#elif defined(RUDIMENTS_HAVE_ARC4RANDOM)
		*result=scaleNumber(arc4random(),0,getRandMax());
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM_R)
		return !random_r(&pvt->buffer,result);
	#elif defined(RUDIMENTS_HAVE_RAND_R)
		pvt->useed=rand_r(&pvt->useed);
		*result=pvt->useed;
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48_R)
		long	res;
		if (lrand48_r(&pvt->buffer,&res)) {
			return false;
		}
		*result=res;
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=random();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_RAND)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=rand();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=lrand48();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#else
		#error "Couldn't find a suitable replacement for rand/srand"
	#endif
}

bool randomnumber::generateScaledNumber(int32_t lower,
						int32_t upper,
						int32_t *result) {
	int32_t	res;
	if (!generateNumber(&res)) {
		return false;
	}
	*result=scaleNumber(res,lower,upper);
	return true;
}

int32_t randomnumber::getSeed() {

	// first try /dev/urandom
	device	d;
	if (d.open("/dev/urandom",O_RDONLY)) {
		int32_t	retval=0;
		if (d.read((void *)&retval,sizeof(retval))==sizeof(retval)) {
			return retval;
		}
	}

	// if that fails, use epoch
	datetime	dt;
	dt.getSystemDateAndTime();
	return (int32_t)dt.getEpoch();
}

int32_t randomnumber::generateNumber(int32_t seed) {
	randomnumber	r;
	int32_t		result;
	return (r.setSeed(seed) &&
		r.generateNumber(&result))?result:-1;
}

int32_t randomnumber::generateScaledNumber(int32_t seed,
					int32_t lower, int32_t upper) {
	randomnumber	r;
	int32_t		result;
	return (r.setSeed(seed) &&
		r.generateScaledNumber(lower,upper,&result))?result:-1;
}

int32_t randomnumber::scaleNumber(int32_t number,
					int32_t lower, int32_t upper) {
	float	originalrange=(int64_t)getRandMax()+1;
	float	newrange=(float)abs(upper-lower)+1.0;
	float	shrunk=((float)number)/originalrange;
	float	expanded=shrunk*newrange;
	int32_t	shifted=lower+(int32_t)expanded;
	return shifted;
}

int32_t randomnumber::getRandMax() {
	return RAND_MAX;
}

bool randomnumber::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_ARC4RANDOM_UNIFORM) && \
		!defined(RUDIMENTS_HAVE_ARC4RANDOM) && \
		!defined(RUDIMENTS_HAVE_RANDOM_R) && \
		!defined(RUDIMENTS_HAVE_RAND_R) && \
		!defined(RUDIMENTS_HAVE_LRAND48_R)
		return true;
	#else
		return false;
	#endif
}

void randomnumber::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_ARC4RANDOM_UNIFORM) && \
		!defined(RUDIMENTS_HAVE_ARC4RANDOM) && \
		!defined(RUDIMENTS_HAVE_RANDOM_R) && \
		!defined(RUDIMENTS_HAVE_RAND_R) && \
		!defined(RUDIMENTS_HAVE_LRAND48_R)
		_rnmutex=mtx;
	#endif
}
