// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/variablebuffer.h>
#include <stdio.h>
#include <string.h>

int main(int argv, const char **argc) {

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

	unsigned char	buffer[5];
	buffer[4]=(unsigned char)NULL;
	vb->setPosition(0);
	unsigned long	sizeread=vb->read(buffer,4);
	printf("%d: ",sizeread);
	for (int i=0; i<sizeread; i++) {
		printf("%c",buffer[i]);
	}
	printf("\n");

	vb->setPosition(5);
	sizeread=vb->read(buffer,4);
	printf("%d: ",sizeread);
	for (int i=0; i<sizeread; i++) {
		printf("%c",buffer[i]);
	}
	printf("\n");

	vb->setPosition(60);
	sizeread=vb->read(buffer,4);
	printf("%d: ",sizeread);
	for (int i=0; i<sizeread; i++) {
		printf("%c",buffer[i]);
	}
	printf("\n");


	delete vb;
}
