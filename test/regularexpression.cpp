// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/regularexpression.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

void printMatches(regularexpression *re) {
	for (int32_t i=0; i<re->getSubstringCount(); i++) {
		printf("match %d start: \"%s\"\n",i,re->getSubstringStart(i));
		printf("match %d end  : \"%s\"\n",i,re->getSubstringEnd(i));
	}
	printf("%d matches\n\n",re->getSubstringCount());
}

int main(int argc, const char **argv) {

        // A quick match...
        if (regularexpression::match("Hello!",".*Dave.*")) {
                printf("The string contains .*Dave.*\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }

        if (regularexpression::match("Hello Dave!",".*Dave.*")) {
                printf("The string contains .*Dave.*\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }

        // If you need to match over and over...
        regularexpression       re("Dave");
        if (re.match("Hello!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Hello Dave!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

	re.study();
        if (re.match("Goodbye!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Goodbye Dave!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

	re.study();
        if (re.match("He is a jerk!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave is a jerk!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("He writes cool software!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave writes cool software!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("See ya later Dave!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("See ya later!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
	}
	printMatches(&re);

        if (re.match("Dave, Dave, Dave!")) {
                printf("The string contains Dave\n");
        } else {
                printf("The string doesn't contain Dave\n");
	}
	printMatches(&re);
}
