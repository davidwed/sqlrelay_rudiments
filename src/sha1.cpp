// Copyright (c) 2016 David Muse
// See the COPYING file for more information

#include <rudiments/sha1.h>
#include <rudiments/bytestring.h>

#include "sha1rfc3174.cpp"

class sha1private {
	friend class sha1;
	private:
		SHA1Context	_context;
		uint8_t		_result[SHA1HashSize+1];
		hasherror_t	_err;
};

sha1::sha1() : hash() {
	pvt=new sha1private;
	clear();
}

sha1::~sha1() {
	delete pvt;
}

bool sha1::append(const unsigned char *data, uint32_t length) {
	pvt->_err=HASH_ERROR_SUCCESS;
	int	result=SHA1Input(&pvt->_context,data,length);
	setError(result);
	return (result==shaSuccess);
}

const unsigned char *sha1::getHash() {
	pvt->_err=HASH_ERROR_SUCCESS;
	int	result=SHA1Result(&pvt->_context,pvt->_result);
	setError(result);
	if (result==shaSuccess) {
		pvt->_result[SHA1HashSize]='\0';
		return pvt->_result;
	}
	return NULL;
}

uint32_t sha1::getHashLength() {
	return SHA1HashSize;
}

bool sha1::clear() {
	pvt->_err=HASH_ERROR_SUCCESS;
	int	result=SHA1Reset(&pvt->_context);
	setError(result);
	bytestring::zero(pvt->_result,sizeof(pvt->_result));
	return (result==shaSuccess);
}

hasherror_t sha1::getError() {
	return pvt->_err;
}

void sha1::setError(int32_t err) {
	switch (err) {
		case shaNull:
			pvt->_err=HASH_ERROR_NULL;
		case shaInputTooLong:
			pvt->_err=HASH_ERROR_INPUT_TOO_LONG;
		case shaStateError:
			pvt->_err=HASH_ERROR_STATE_ERROR;
		default:
			pvt->_err=HASH_ERROR_SUCCESS;
	}
}
