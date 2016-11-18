// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/binarytree.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("binarytree");

	// integers
	stdoutput.printf("integers:\n");
	binarytree<int32_t>	intt;

	// insert
	intt.insert((int32_t)0);
	intt.insert((int32_t)1);
	intt.insert((int32_t)3);
}
