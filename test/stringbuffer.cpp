// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

uint32_t	iterations=5;
uint32_t	members=1024;

char	alphabet[]="abdefghijklmnopqrstuvwxyz";

int main(int argc, const char **argv) {

	header("stringbuffer");


	// create a new string buffer
	stringbuffer	*strb=new stringbuffer();
	char		str[800000];
	char		buf[64];

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,sizeof(str));

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->append((const char *)NULL);

			// empty string
			strb->append("");

			// strings
			for (uint16_t k=1; k<=25; k++) {
				strb->append(alphabet,k);
				charstring::append(str,alphabet,k);
			}

			// integers
			for (uint64_t k=1; k<10000; k*=10) {
				strb->append((uint16_t)k);
				charstring::printf(buf,sizeof(buf),
							"%hd",(uint16_t)k);
				charstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000; k*=10) {
				strb->append((uint32_t)k);
				charstring::printf(buf,sizeof(buf),
							"%ld",(uint32_t)k);
				charstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000000000000; k*=10) {
				strb->append((uint64_t)k);
				charstring::printf(buf,sizeof(buf),
							"%lld",(uint64_t)k);
				charstring::append(str,buf);
			}

			// float
			float	mult=10;
			for (uint16_t k=1; k<5; k++) {
				float	l=k+((float)k/mult);
				mult*=10;
				strb->append(l,k+1,l);
				charstring::printf(buf,sizeof(buf),
							"%*.*f",k+1,k,l);
				charstring::append(str,buf);
			}

			// double
			mult=10;
			for (uint16_t k=1; k<5; k++) {
				double	l=k+((double)k/mult);
				mult*=10;
				strb->append(l,k+1,k);
				charstring::printf(buf,sizeof(buf),
							"%*.*f",k+1,k,l);
				charstring::append(str,buf);
			}
		}

		test("length",charstring::length(str)==strb->getStringLength());
		test("contents",!charstring::compare(str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	for (uint16_t i=0; i<iterations; i++) {
		for (uint16_t j=0; j<10000; j++) {
			strb->setPosition(j*(30+i));
			strb->write("66666");
			charstring::copy(str+(j*(30+i)),"66666",5);
		}
		test("contents",!charstring::compare(str,strb->getString()));
	}
	delete strb;
	stdoutput.printf("\n");


	// initial contents...
	/*stdoutput.printf("initial contents...\n");
	for (uint16_t i=0; i<iterations; i++) {

		bytestring::zero(str,sizeof(str));

		char	*data=new char[128*i];
		bytestring::zero(data,128*i);
		for (uint16_t j=0; j<128*i/16; j++) {
			charstring::append(data,"0123456789abcdef");
			charstring::append(str,"0123456789abcdef");
		}

		strb=new stringbuffer(data,charstring::length(data),32);
		for (uint16_t j=0; j<=i*100; j++) {
			strb->append("0123456789abcdef");
			charstring::append(str,"0123456789abcdef");
		}

		test("contents",!charstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");*/
}
