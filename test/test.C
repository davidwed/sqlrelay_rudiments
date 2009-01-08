#include <stdio.h>
#include <stdlib.h>

void	test(const char *printstring, bool result) {
	printf("\t");
	printf(printstring);
	printf((result)?": success":": failed");
	printf("\n");
	if (!result) {
		exit(0);
	}
}
