// Copyright (c) 2014  David Muse
// See the file COPYING for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	const char	*salts[]={
		"FW",
		"fw",
		NULL
	};

	const char	*unencrypted[]={
		"rudiments",
		"RUDIMENTS",
		"firstworks",
		"FIRSTWORKS",
		NULL
	};

	const char	*encrypted[][4]={
		{
		"FWR60yTxIvBZo",
		"FWlt9zHgfs4j2",
		"FW8vxm0D2fdZQ",
		"FWBVR0cwLQHEw",
		},
		{
		"fwdzejOvuN/mc",
		"fwNiqldtGLMeI",
		"fwaWWmdtSjxcA",
		"fwDHL5QynIMNg",
		}
	};

	uint16_t	i=0;
	for (const char * const *salt=salts; *salt; salt++) {

		stdoutput.printf("salt=%s\n",*salt);

		uint16_t	j=0;
		for (const char * const *str=unencrypted; *str; str++) {
			char	*enc=crypt::encrypt(*str,*salt);
			test("",!charstring::compare(enc,encrypted[i][j]));
			delete[] enc;
			j++;
		}
		i++;
	}
}
