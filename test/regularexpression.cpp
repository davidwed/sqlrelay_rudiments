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
        const char	*str="Hello!";
        const char	*mtc="Dave";
        if (regularexpression::match(str,mtc)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }

	str="Hello Dave!";
        if (regularexpression::match(str,mtc)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}

        // If you need to match over and over...
        regularexpression       re;
	re.compile(mtc);
        str="Hello!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

	str="Hello Dave!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

	re.study();
	str="Goodbye!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

	str="Goodbye Dave!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

	re.study();
	str="He is a jerk!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

	str="Dave is a jerk!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

        str="He writes cool software!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
	}
	printMatches(&re);

        str="Dave writes cool software!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	str="See ya later Dave!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	str="See ya later!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	str="Dave, Dave, Dave!";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	mtc="(\\w+) (\\w+)";
	re.compile(mtc);
	str="hello world";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	mtc="(\\w+) (\\w+)";
	re.compile(mtc);
	str="hello world hello world";
        if (re.match(str)) {
                stdoutput.printf(
			"The string \"%s\" contains \"%s\"\n",str,mtc);
        } else {
                stdoutput.printf(
			"The string \"%s\" doesn't contain \"%s\"\n",str,mtc);
        }
	printMatches(&re);

	// NULL pattern
	re.compile(NULL);
	re.study();
        if (re.match(NULL)) {
                stdoutput.printf("The string NULL contains NULL\n");
	} else {
                stdoutput.printf("The string NULL does not contain NULL\n");
	}
	printMatches(&re);
}
