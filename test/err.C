// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/errorhandler.h>
#include <stdio.h>

class myclass : public errorhandler {
	public:
		void	test1();
		void	test2();
		void	test3();
		void	test4();
};

void	myclass::test1() {
	clearError();
	appendError("error1");
}

void	myclass::test2() {
	clearError();
	appendError("error2");
}

void	myclass::test3() {
	clearError();
	appendError("error3");
}

void	myclass::test4() {
	clearError();
	appendError("error4");
}

int main(int argv, const char **argc) {
	myclass	m;
	m.test1();
	printf("%s\n",m.getError());
	m.test2();
	printf("%s\n",m.getError());
	m.test3();
	printf("%s\n",m.getError());
	m.test4();
	printf("%s\n",m.getError());
}
