#include <rudiments/crypt.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char * const salts[]={
		"FW",
		"fw",
		NULL
	};

	const char * const strings[]={
		"rudiments",
		"RUDIMENTS",
		"firstworks",
		"FIRSTWORKS",
		NULL
	};

	for (const char * const *salt=salts; *salt; salt++) {

		stdoutput.printf("salt=%s\n",*salt);

		for (const char * const *str=strings; *str; str++) {

			char	*encrypted=crypt::encrypt(*str,*salt);

			stdoutput.printf("  %s: %s\n",*str,encrypted);
			delete[] encrypted;
		}
	}
}
