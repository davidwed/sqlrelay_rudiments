// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/regularexpression.h>
#include <stdio.h>

void printMatches(regularexpression *re) {
	#if defined(RUDIMENTS_HAS_PCRE) || defined(HAVE_REGEX_H)
	printf("%d matches\n",re->getSubstringCount());
	for (int i=0; i<re->getSubstringCount(); i++) {
		printf("match: \"%s\"\n",re->getSubstringStart(i));
	}
	#endif
}

int main(int argc, const char **argv) {

        // A quick match...
        if (regularexpression::match("Hello Dave!",".*Dave.*")) {
                printf("The string contains .*Dave.*\n");
        }

        // If you need to match over and over...
        regularexpression       re("Dave");
        if (re.match("Hello Dave!")) {
                printf("The string contains Dave\n");
        }
	printMatches(&re);

        if (re.match("Goodbye Dave!")) {
                printf("The string contains Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave is a jerk!")) {
                printf("The string contains Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave writes cool software!")) {
                printf("The string contains Dave\n");
        }
	printMatches(&re);

        if (re.match("See ya later Dave!")) {
                printf("The string contains Dave\n");
        }
	printMatches(&re);
}
