// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/variablebuffer.h>
#include <stdio.h>
#include <string.h>

int	main() {

	variablebuffer	*vb=new variablebuffer(10,10);

	vb->append((unsigned char *)"12345",5);
	vb->append((unsigned char *)"12345",5);
	vb->append((unsigned char *)"12345",5);
	vb->append((unsigned char *)"12345",5);
	vb->append((unsigned char *)"12345",5);
	for (int i=0; i<vb->getSize(); i++) {
		printf("%c",vb->getBuffer()[i]);
	}
	printf("\n");

	vb->setPosition(0);
	vb->write((unsigned char *)"66666",5);
	for (int i=0; i<vb->getSize(); i++) {
		printf("%c",vb->getBuffer()[i]);
	}
	printf("\n");

	vb->setPosition(30);
	vb->write((unsigned char *)"66666",5);
	for (int i=0; i<vb->getSize(); i++) {
		if (vb->getBuffer()[i]>=' ' && vb->getBuffer()[i]<='~') {
			printf("%c",vb->getBuffer()[i]);
		} else {
			printf(".");
		}
	}
	printf("\n");

	vb->setPosition(50);
	vb->append((unsigned char *)"12345",5);
	for (int i=0; i<vb->getSize(); i++) {
		if (vb->getBuffer()[i]>=' ' && vb->getBuffer()[i]<='~') {
			printf("%c",vb->getBuffer()[i]);
		} else {
			printf(".");
		}
	}
	printf("\n");

	vb->write((unsigned char *)"12345",5);
	for (int i=0; i<vb->getSize(); i++) {
		if (vb->getBuffer()[i]>=' ' && vb->getBuffer()[i]<='~') {
			printf("%c",vb->getBuffer()[i]);
		} else {
			printf(".");
		}
	}
	printf("\n");

	delete vb;
}
