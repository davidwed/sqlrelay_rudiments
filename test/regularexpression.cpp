// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/regularexpression.h>
#include <rudiments/stdio.h>

void printMatches(regularexpression *re) {
	for (int32_t i=0; i<re->getSubstringCount(); i++) {
		stdoutput.printf("match %d start: \"%s\"\n",
					i,re->getSubstringStart(i));
		stdoutput.printf("match %d end  : \"%s\"\n",
					i,re->getSubstringEnd(i));
	}
	stdoutput.printf("%d matches\n\n",re->getSubstringCount());
}

int main(int argc, const char **argv) {

        // A quick match...
        if (regularexpression::match("Hello!",".*Dave.*")) {
                stdoutput.printf("The string contains .*Dave.*\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }

        if (regularexpression::match("Hello Dave!",".*Dave.*")) {
                stdoutput.printf("The string contains .*Dave.*\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }

        // If you need to match over and over...
        regularexpression       re("Dave");
        if (re.match("Hello!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Hello Dave!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

	re.study();
        if (re.match("Goodbye!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Goodbye Dave!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

	re.study();
        if (re.match("He is a jerk!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave is a jerk!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("He writes cool software!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("Dave writes cool software!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("See ya later Dave!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
        }
	printMatches(&re);

        if (re.match("See ya later!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
	}
	printMatches(&re);

        if (re.match("Dave, Dave, Dave!")) {
                stdoutput.printf("The string contains Dave\n");
        } else {
                stdoutput.printf("The string doesn't contain Dave\n");
	}
	printMatches(&re);
}
