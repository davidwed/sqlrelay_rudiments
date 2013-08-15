// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include "test.cpp"

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif


int main(int argc, const char **argv) {

	printf("charstring:\n");

	// create a new string 50 bytes long
	char	s[50];


	// set every byte in the string to NULL
	charstring::zero(s,50);
	bool	zeros=true;
	for (int i=0; i<50; i++) {
		if (s[i]!=0) {
			zeros=false;
		}
	}
	test("zeros",zeros);


	// Append "hello there dave" to the string and display it.
	// The 3rd call to append() only appends the first 4 bytes of "dave !!!"
	charstring::append(s,"hello ");
	charstring::append(s,"there ");
	charstring::append(s,"dave !!!",4);
	printf("\"%s\"\n",s);


	// Replace the contents of the string and display it over and over.
	// Note that the 2nd and 4th call to copy() only copy part of the
	// string passed into them.
	charstring::copy(s,"what's ");
	printf("\"%s\"\n",s);
	charstring::copy(s,"up !!!",2);
	printf("\"%s\"\n",s);
	charstring::copy(s,2," !!!");
	printf("\"%s\"\n",s);
	charstring::copy(s,6,"!!!!!!",1);
	printf("\"%s\"\n",s);


	// clear the string again.
	charstring::zero(s,50);


	// Append "hello" to the string.
	charstring::append(s,"hello");

	// perform several comparisons, all of which should return 0
	printf("compare: %d=0\n",charstring::compare(s,"hello"));
	printf("compare: %d=0\n",charstring::compare(s,"hello",3));
	printf("compare: %d=0\n",charstring::compareIgnoringCase(s,"HELLO"));
	printf("compare: %d=0\n",charstring::compareIgnoringCase(s,"HELLO",3));

	// perform several contains() comparisons
	printf("findFirst: \"%s\"=llo\n",charstring::findFirst(s,"llo"));
	printf("contains: %d\n",charstring::contains(s,"llo"));
	printf("findFirst: \"%s\"=llo\n",charstring::findFirst(s,'l'));
	printf("contains: %d\n",charstring::contains(s,"llo"));



	// duplicate the string and display the duplicated string
	char	*hellothere=charstring::duplicate(s);
	char	*ell=charstring::subString(hellothere,1,3);
	char	*lle=charstring::subString(hellothere,3,1);
	printf("hello: %s\n",hellothere);
	printf("ell: %s\n",ell);
	printf("ell: %s\n",lle);

	// make sure to clean up what duplicate() returns
	delete[] hellothere;
	delete[] ell;
	delete[] lle;


	// split
	char		**list;
	uint64_t	listlength;
	charstring::split("hello||hi||bye||goodbye","||",false,
						&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	printf("split(\"hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	printf("split(\"||hello||hi||bye||goodbye||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||hello||||hi||||bye||||goodbye||||","||",false,
							&list,&listlength);
	printf("split(\"||||hello||||hi||||bye||||goodbye||||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||||||||","||",false,&list,&listlength);
	printf("split(\"||||||||||\",\"||\")\n");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	charstring::split("http://www.firstworks.com/application/app.cgi/skin/module/template.html","/",false,&list,&listlength);
	printf("split(\"http://www.firstworks.com/application/app.cgi/skin/module/template.html\",\"/\"");
	printf("%lld items\n",listlength);
	for (uint64_t i=0; i<listlength; i++) {
		printf("	%s\n",list[i]);
		delete[] list[i];
	}
	delete[] list;

	char	str[]="hello'\"\\hello'\"\\";
	char	*escapedstr=charstring::escape(str,"\"'");
	char	*unescapedstr=charstring::unescape(escapedstr);
	printf("str		: %s\n",str);
	printf("escapedstr	: %s\n",escapedstr);
	printf("unescapedstr	: %s\n",unescapedstr);
	delete[] unescapedstr;
	delete[] escapedstr;

	// from Thomas Hobbes's Leviathan...
	unsigned char	base64str1[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	unsigned char	base64str2[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure";
	unsigned char	base64str3[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasur";
	unsigned char	base64str4[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasu";

	char	*encoded1=charstring::base64Encode(base64str1);
	printf("str:\n%s\n",base64str1);
	printf("encoded:\n%s\n",encoded1);
	unsigned char	*decoded1=charstring::base64Decode(encoded1);
	printf("decoded:\n%s\n",decoded1);
	printf("same as original? %d\n",
		!charstring::compare((char *)decoded1,(char *)base64str1));
	delete[] encoded1;
	delete[] decoded1;

	char	*encoded2=charstring::base64Encode(base64str2);
	printf("str:\n%s\n",base64str2);
	printf("encoded:\n%s\n",encoded2);
	unsigned char	*decoded2=charstring::base64Decode(encoded2);
	printf("decoded:\n%s\n",decoded2);
	printf("same as original? %d\n",
		!charstring::compare((char *)decoded2,(char *)base64str2));
	delete[] encoded2;
	delete[] decoded2;

	char	*encoded3=charstring::base64Encode(base64str3);
	printf("str:\n%s\n",base64str3);
	printf("encoded:\n%s\n",encoded3);
	unsigned char	*decoded3=charstring::base64Decode(encoded3);
	printf("decoded:\n%s\n",decoded3);
	printf("same as original? %d\n",
		!charstring::compare((char *)decoded3,(char *)base64str3));
	delete[] encoded3;
	delete[] decoded3;

	char	*encoded4=charstring::base64Encode(base64str4);
	printf("str:\n%s\n",base64str4);
	printf("encoded:\n%s\n",encoded4);
	unsigned char	*decoded4=charstring::base64Decode(encoded4);
	printf("decoded:\n%s\n",decoded4);
	printf("same as original? %d\n",
		!charstring::compare((char *)decoded4,(char *)base64str4));
	delete[] encoded4;
	delete[] decoded4;

	unsigned char	bytes[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	char		*encodedbytes;
	uint64_t	encodedlen;
	unsigned char	*decodedbytes;
	uint64_t	decodedlen;
	charstring::base64Encode(bytes,6,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,6);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;
	charstring::base64Encode(bytes,5,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,5);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;
	charstring::base64Encode(bytes,4,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,4);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;
	charstring::base64Encode(bytes,3,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,3);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;
	charstring::base64Encode(bytes,2,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,2);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;
	charstring::base64Encode(bytes,1,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	printf("bytes: \"");
	stdoutput.safePrint((char *)bytes,1);
	printf("\"\n");
	printf("encoded bytes: (%lld) \"%s\"\n",encodedlen,encodedbytes);
	printf("decoded bytes: \"");
	stdoutput.safePrint((char *)decodedbytes,decodedlen);
	printf("\"\n");
	delete[] encodedbytes;
	delete[] decodedbytes;

	
	const char	*alphabet="aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	printf("lengthContainingSet(\"%s\",\"gfedcba\")=%d\n",
		alphabet,charstring::lengthContainingSet(alphabet,"gfedcba"));
	printf("lengthNotContainingSet(\"%s\",\"hijklmnopqrstuvwxyz\")=%d\n",
		alphabet,charstring::lengthNotContainingSet(alphabet,
						"hijklmnopqrstuvwxyz"));


	printf("findFirstOfSet(\"%s\",\"mlk\")=\"%s\"\n",
		alphabet,charstring::findFirstOfSet(alphabet,"klm"));


	// initialize a text buffer and print it out
	char    buffer[100];
	charstring::copy(buffer,"     hello there buddy     ");
	printf("!%s!\n",buffer);


	// trim the spaces off of the right hand side
	charstring::rightTrim(buffer);
	printf("!%s!\n",buffer);


	// trim the spaces off of the left hand side
	charstring::leftTrim(buffer);
	printf("!%s!\n",buffer);


	// strip the spaces out
	charstring::copy(buffer,"   hello      there   buddy  ");
	charstring::strip(buffer,' ');
	printf("!%s!\n",buffer);


	// strip the "   " out
	charstring::copy(buffer,"   hello       there  buddy  ");
	charstring::strip(buffer,"   ");
	printf("!%s!\n",buffer);


	// convert buffer to uppercase
	charstring::upper(buffer);
	printf("!%s!\n",buffer);


	// convert buffer to lowercase
	charstring::lower(buffer);
	printf("!%s!\n",buffer);


	// http escape the buffer
	char    *escbuffer=charstring::httpEscape("!@#$%^&*()hello-+");
	printf("!@#$%%^&*()hello-+  http escaped is  %s\n",escbuffer);
	char	*unescbuffer=charstring::httpUnescape(escbuffer);
	printf("http unescaped is  %s\n",unescbuffer);
	delete escbuffer;
	delete unescbuffer;
	

	// evaluate a string to see if it's a number
	if (charstring::isNumber("-100.5")) {
		printf("-100.5 is a number\n");
	}
	if (!charstring::isNumber("-100.5.10")) {
		printf("-100.5.10 is not a number\n");
	}
	

	// evaluate a string to see if it's an integer
	if (charstring::isInteger("-100")) {
		printf("-100 is an integer\n");
	}
	if (!charstring::isInteger("-100.5")) {
		printf("-100.5.10 is not an integer\n");
	}


	// create a new string containing text surrounded by spaces
	char	hello[16];
	charstring::copy(hello,"     hello     ");
	printf("|%s|\n",hello);

	// left justify the text
	charstring::leftJustify(hello,15);
	printf("|%s|\n",hello);

	// right justify the text
	charstring::rightJustify(hello,15);
	printf("|%s|\n",hello);


	// center the text over and over...
	charstring::copy(hello,"hello          ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello," hello         ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"  hello        ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"   hello       ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"    hello      ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"     hello     ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"      hello    ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"       hello   ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"        hello  ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"         hello ");
	charstring::center(hello,15);
	printf("|%s|\n",hello);
	charstring::copy(hello,"          hello");
	charstring::center(hello,15);
	printf("|%s|\n",hello);


	// print the number of bytes necessary to store each number as a string
	printf("size of 1 is: %d\n",charstring::integerLength((int64_t)1));
	printf("size of 10 is: %d\n",charstring::integerLength((int64_t)10));
	printf("size of 100 is: %d\n",charstring::integerLength((int64_t)100));
	printf("size of 1000 is: %d\n",charstring::integerLength((int64_t)1000));
	printf("size of -1 is: %d\n",charstring::integerLength((int64_t)-1));
	printf("size of -10 is: %d\n",charstring::integerLength((int64_t)-10));
	printf("size of -100 is: %d\n",charstring::integerLength((int64_t)-100));
	printf("size of -1000 is: %d\n",charstring::integerLength((int64_t)-1000));


	uint64_t	testuint64=ULONG_MAX;
	char		*teststr=charstring::parseNumber(testuint64);
	printf("18446744073709551615=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testuint64,40);
	printf("18446744073709551615=%s\n",teststr);
	delete[] teststr;

	int64_t		testint64=LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	printf("9223372036854775807=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	printf("9223372036854775807=%s\n",teststr);
	testint64=-1*LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	printf("-9223372036854775807=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	printf("-9223372036854775807=%s\n",teststr);
	delete[] teststr;

	uint32_t	testuint32=4294967295UL;
	teststr=charstring::parseNumber(testuint32);
	printf("4294967295=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testuint32,40);
	printf("4294967295=%s\n",teststr);
	delete[] teststr;

	int32_t		testint32=2147483647;
	teststr=charstring::parseNumber(testint32);
	printf("2147483647=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	printf("2147483647=%s\n",teststr);
	testint32=-2147483647;
	teststr=charstring::parseNumber(testint32);
	printf("-2147483647=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	printf("-2147483647=%s\n",teststr);
	delete[] teststr;

	uint16_t	testuint16=65535;
	teststr=charstring::parseNumber(testuint16);
	printf("65535=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testuint16,40);
	printf("65535=%s\n",teststr);
	delete[] teststr;

	int16_t		testint16=32767;
	teststr=charstring::parseNumber(testint16);
	printf("32767=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	printf("32767=%s\n",teststr);
	testint16=-32767;
	teststr=charstring::parseNumber(testint16);
	printf("-32767=%s\n",teststr);
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	printf("-32767=%s\n",teststr);
	delete[] teststr;

	unsigned char	v8=0xC0;
	uint16_t	v16=filedescriptor::hostToNet(
						(uint16_t)0xC0C0);
	uint32_t	v32=filedescriptor::hostToNet(
						(uint32_t)0xC0C0C0C0);
	uint64_t	v64=filedescriptor::hostToNet(
					(uint64_t)0xC0C0C0C0C0C0C0C0ULL);
	char	sv8=-1*0x01;
	int16_t	sv16=filedescriptor::hostToNet((uint16_t)
						(int16_t)(-1*0x0101));
	int32_t	sv32=filedescriptor::hostToNet((uint32_t)
						(int32_t)(-1*0x01010101));
	int64_t	sv64=filedescriptor::hostToNet((uint64_t)
					(int64_t)(-1*0x0101010101010101LL));
	unsigned char	v8s[]={0xC0,0xC0,0xC0,0xC0};
	stdoutput.printBits(v8);
	printf("\n");
	stdoutput.printBits(v16);
	printf("\n");
	stdoutput.printBits(v32);
	printf("\n");
	stdoutput.printBits(v64);
	printf("\n");
	stdoutput.printBits(sv8);
	printf("\n");
	stdoutput.printBits(sv16);
	printf("\n");
	stdoutput.printBits(sv32);
	printf("\n");
	stdoutput.printBits(sv64);
	printf("\n");
	stdoutput.printBits(v8s,sizeof(v8s));
	printf("\n");

}
