// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/mutex.h>

mutex::mutex() {
	pthread_mutex_init(mut,NULL);
}

mutex::mutex(pthread_mutex_t *mutex) {
	this->mut=mut;
}

mutex::~mutex() {
	pthread_mutex_destroy(mut);
}

bool mutex::lock() {
	return !pthread_mutex_lock(mut);
}

bool mutex::tryLock() {
	return !pthread_mutex_trylock(mut);
}

bool mutex::unlock() {
	return !pthread_mutex_unlock(mut);
}

pthread_mutex_t *mutex::getMutex() {
	return mut;
}
