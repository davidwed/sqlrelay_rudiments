// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("charstring");

	// zero
	stdoutput.printf("zero...\n");
	char	s[50];
	charstring::zero(s,50);
	bool	zeros=true;
	for (int zi=0; zi<50; zi++) {
		if (s[zi]!=0) {
			zeros=false;
		}
	}
	test("zeros",zeros);
	stdoutput.printf("\n");

	// append
	stdoutput.printf("append...\n");
	charstring::append(s,"hello ");
	charstring::append(s,"there ");
	charstring::append(s,"dave !!!",4);
	test("append",!charstring::compare(s,"hello there dave"));
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	charstring::copy(s,"what's ");
	test("append",!charstring::compare(s,"what's "));
	charstring::copy(s,"up !!!",2);
	test("append",!charstring::compare(s,"upat's "));
	charstring::copy(s,2," !!!");
	test("append",!charstring::compare(s,"up !!!"));
	charstring::copy(s,6,"!!!!!!",1);
	test("append",!charstring::compare(s,"up !!!!"));
	stdoutput.printf("\n");


	// compare
	stdoutput.printf("compare...\n");
	charstring::zero(s,50);
	charstring::append(s,"hello");
	test("compare",!charstring::compare(s,"hello"));
	test("compare",!charstring::compare(s,"hello",3));
	test("compare",!charstring::compareIgnoringCase(s,"HELLO"));
	test("compare",!charstring::compareIgnoringCase(s,"HELLO",3));
	stdoutput.printf("\n");


	// findFirst/contains
	stdoutput.printf("findFirst/contains...\n");
	test("findFirst",charstring::findFirst(s,"llo"));
	test("contains",charstring::contains(s,"llo"));
	test("findFirst",charstring::findFirst(s,'l'));
	test("contains",charstring::contains(s,"llo"));
	stdoutput.printf("\n");


	// duplicate
	stdoutput.printf("duplicate/subString...\n");
	char	*hellothere=charstring::duplicate(s);
	test("duplicate",!charstring::compare(hellothere,"hello"));
	char	*ell=charstring::subString(hellothere,1,3);
	test("subString",!charstring::compare(ell,"ell"));
	char	*llo=charstring::subString(hellothere,2,4);
	test("subString",!charstring::compare(llo,"llo"));
	delete[] hellothere;
	delete[] ell;
	delete[] llo;
	stdoutput.printf("\n");


	// printf
	stdoutput.printf("printf...\n");
	char	buffer[100];
	ssize_t	size=charstring::printf(buffer,20,"%c%d%s",'a',123,"hello");
	test("size",size==9);
	test("contents",!charstring::compare(buffer,"a123hello"));
	char	*buf=NULL;
	size=charstring::printf(&buf,"%c%d%s",'a',123,"hello");
	test("size",size==9);
	test("contents",!charstring::compare(buffer,"a123hello"));
	delete[] buf;
	stdoutput.printf("\n");


	// split
	stdoutput.printf("split...\n");
	char		**list;
	uint64_t	listlength;
	charstring::split("hello||hi||bye||goodbye","||",false,
						&list,&listlength);
	test("length",listlength==4);
	test("list[0]",!charstring::compare(list[0],"hello"));
	test("list[1]",!charstring::compare(list[1],"hi"));
	test("list[2]",!charstring::compare(list[2],"bye"));
	test("list[3]",!charstring::compare(list[3],"goodbye"));
	uint64_t i;
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	test("length",listlength==5);
	test("list[0]",!charstring::compare(list[0],"hello"));
	test("list[1]",!charstring::compare(list[1],"hi"));
	test("list[2]",!charstring::compare(list[2],"bye"));
	test("list[3]",!charstring::compare(list[3],"goodbye"));
	test("list[4]",!charstring::compare(list[4],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],"hello"));
	test("list[2]",!charstring::compare(list[2],"hi"));
	test("list[3]",!charstring::compare(list[3],"bye"));
	test("list[4]",!charstring::compare(list[4],"goodbye"));
	test("list[5]",!charstring::compare(list[5],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||hello||||hi||||bye||||goodbye||||","||",false,
							&list,&listlength);
	test("length",listlength==11);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],""));
	test("list[2]",!charstring::compare(list[2],"hello"));
	test("list[3]",!charstring::compare(list[3],""));
	test("list[4]",!charstring::compare(list[4],"hi"));
	test("list[5]",!charstring::compare(list[5],""));
	test("list[6]",!charstring::compare(list[6],"bye"));
	test("list[7]",!charstring::compare(list[7],""));
	test("list[8]",!charstring::compare(list[8],"goodbye"));
	test("list[9]",!charstring::compare(list[9],""));
	test("list[10]",!charstring::compare(list[10],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||||||||","||",false,&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],""));
	test("list[2]",!charstring::compare(list[2],""));
	test("list[3]",!charstring::compare(list[3],""));
	test("list[4]",!charstring::compare(list[4],""));
	test("list[5]",!charstring::compare(list[5],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("http://www.firstworks.com/application/app.cgi/skin/module/template.html","/",true,&list,&listlength);
	test("length",listlength==7);
	test("list[0]",!charstring::compare(list[0],"http:"));
	test("list[1]",!charstring::compare(list[1],"www.firstworks.com"));
	test("list[2]",!charstring::compare(list[2],"application"));
	test("list[3]",!charstring::compare(list[3],"app.cgi"));
	test("list[4]",!charstring::compare(list[4],"skin"));
	test("list[5]",!charstring::compare(list[5],"module"));
	test("list[6]",!charstring::compare(list[6],"template.html"));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("1.2.3.4.5.6",".",false,NULL,&listlength);
	test("length",listlength==6);

	// this test just shouldn't crash
	charstring::split("1.2.3.4.5.6",".",false,NULL,NULL);
	stdoutput.printf("\n");


	// escape/unescape
	stdoutput.printf("escape/unescape...\n");
	char	str[]="hello'\"\\hello'\"\\";
	char	*escapedstr=charstring::escape(str,"\"'");
	test("escape",!charstring::compare(escapedstr,
					"hello\\'\\\"\\\\hello\\'\\\"\\\\"));
	char	*unescapedstr=charstring::unescape(escapedstr);
	test("unescape",!charstring::compare(unescapedstr,
					"hello'\"\\hello'\"\\"));
	delete[] unescapedstr;
	delete[] escapedstr;
	stdoutput.printf("\n");


	// base64
	stdoutput.printf("base64...\n");
	// from Thomas Hobbes's Leviathan...
	unsigned char	base64str1[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	unsigned char	base64str2[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure";
	unsigned char	base64str3[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasur";
	unsigned char	base64str4[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasu";

	char	*encoded1=charstring::base64Encode(base64str1);
	unsigned char	*decoded1=charstring::base64Decode(encoded1);
	test("base64 test 1",
		!charstring::compare((char *)decoded1,(char *)base64str1));
	delete[] encoded1;
	delete[] decoded1;

	char	*encoded2=charstring::base64Encode(base64str2);
	unsigned char	*decoded2=charstring::base64Decode(encoded2);
	test("base64 test 2",
		!charstring::compare((char *)decoded2,(char *)base64str2));
	delete[] encoded2;
	delete[] decoded2;

	char	*encoded3=charstring::base64Encode(base64str3);
	unsigned char	*decoded3=charstring::base64Decode(encoded3);
	test("base64 test 3",
		!charstring::compare((char *)decoded3,(char *)base64str3));
	delete[] encoded3;
	delete[] decoded3;

	char	*encoded4=charstring::base64Encode(base64str4);
	unsigned char	*decoded4=charstring::base64Decode(encoded4);
	test("base64 test 4",
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
	test("base64 test 5",
		!bytestring::compare(decodedbytes,bytes,6));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,5,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 6",
		!bytestring::compare(decodedbytes,bytes,5));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,4,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 7",
		!bytestring::compare(decodedbytes,bytes,4));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,3,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 8",
		!bytestring::compare(decodedbytes,bytes,3));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,2,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 9",
		!bytestring::compare(decodedbytes,bytes,2));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,1,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 10",
		!bytestring::compare(decodedbytes,bytes,1));
	delete[] encodedbytes;
	delete[] decodedbytes;
	stdoutput.printf("\n");


	// http escape the buffer
	stdoutput.printf("http-escape...\n");
	const char	*original="!@#$%^&*()hello-+";
	char    *escbuffer=charstring::httpEscape("!@#$%^&*()hello-+");
	test("escaped",!charstring::compare(escbuffer,
					"!@#$%25%5E&*()hello-%2B"));
	char	*unescbuffer=charstring::httpUnescape(escbuffer);
	test("unescaped",!charstring::compare(unescbuffer,original));
	delete[] escbuffer;
	delete[] unescbuffer;
	stdoutput.printf("\n");


	// sets
	stdoutput.printf("sets...\n");
	const char	*alphabet=
		"aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	test("lengthContainingSet",
		charstring::lengthContainingSet(
				alphabet,"gfedcba")==14);
	test("lengthNotContainingSet",
		charstring::lengthNotContainingSet(
				alphabet,"hijklmnopqrstuvwxyz")==14);
	test("findFirstOfSet",charstring::findFirstOfSet(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd",
			charstring::findFirstOfSetOrEnd(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd (end)",
			charstring::findFirstOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::length(alphabet)));
	test("findLastOfSetOrEnd",
			charstring::findLastOfSetOrEnd(alphabet,"abc")==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
			charstring::findLastOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::length(alphabet)));
	stdoutput.printf("\n");


	// trim
	stdoutput.printf("trim...\n");
	charstring::copy(buffer,"\r\n	     hello there buddy     	\r\n");
	charstring::rightTrim(buffer);
	test("rtrim",!charstring::compare(buffer,
				"\r\n	     hello there buddy"));
	charstring::leftTrim(buffer);
	test("ltrim",!charstring::compare(buffer,"hello there buddy"));
	stdoutput.printf("\n");


	// strip
	stdoutput.printf("strip...\n");
	charstring::copy(buffer,"   hello      there   buddy  ");
	charstring::strip(buffer,' ');
	test("strip",!charstring::compare(buffer,"hellotherebuddy"));
	charstring::copy(buffer,"   hello       there  buddy  ");
	charstring::strip(buffer,"   ");
	test("strip",!charstring::compare(buffer,"hello there  buddy  "));
	stdoutput.printf("\n");


	// justification
	stdoutput.printf("justification...\n");
	char	hello[16];
	charstring::copy(hello,"     hello     ");
	charstring::leftJustify(hello,15);
	test("left",!charstring::compare(hello,"hello          "));
	charstring::rightJustify(hello,15);
	test("right",!charstring::compare(hello,"          hello"));
	charstring::copy(hello,"hello          ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello," hello         ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"  hello        ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"   hello       ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"    hello      ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"     hello     ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"      hello    ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"       hello   ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"        hello  ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"         hello ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"          hello");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	stdoutput.printf("\n");


	// case conversion
	stdoutput.printf("case conversion...\n");
	charstring::upper(buffer);
	test("upper",!charstring::compare(buffer,"HELLO THERE  BUDDY  "));
	charstring::lower(buffer);
	test("lower",!charstring::compare(buffer,"hello there  buddy  "));
	charstring::capitalize(buffer);
	test("caps",!charstring::compare(buffer,"Hello There  Buddy  "));
	stdoutput.printf("\n");
	

	// numbers
	stdoutput.printf("number...\n");
	test("isNumber: -100.5",charstring::isNumber("-100.5"));
	test("isNumber: -100.5.10",!charstring::isNumber("-100.5.10"));
	test("isInteger: -100",charstring::isInteger("-100"));
	test("isInteger: -100.5",!charstring::isInteger("-100.5"));
	test("isInteger: -100.5.10",!charstring::isInteger("-100.5.10"));
	test("integerLength: 1",
			charstring::integerLength((int64_t)1)==1);
	test("integerLength: 10",
			charstring::integerLength((int64_t)10)==2);
	test("integerLength: 100",
			charstring::integerLength((int64_t)100)==3);
	test("integerLength: 1000",
			charstring::integerLength((int64_t)1000)==4);
	test("integerLength: -1",
			charstring::integerLength((int64_t)-1)==2);
	test("integerLength: -10",
			charstring::integerLength((int64_t)-10)==3);
	test("integerLength: -100",
			charstring::integerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			charstring::integerLength((int64_t)-1000)==5);

	uint64_t	testuint64=ULONG_MAX;
	char		*teststr=charstring::parseNumber(testuint64);
	test("parseNumber ULONG_MAX",
		!charstring::compare(teststr,
			(testuint64==4294967295UL)
				?"4294967295"
				:"18446744073709551615"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint64,40);
	test("parseNumber ULONG_MAX",
		!charstring::compare(teststr,
			(testuint64==4294967295UL)
				?"0000000000000000000000000000004294967295"
				:"0000000000000000000018446744073709551615"));
	delete[] teststr;

	int64_t		testint64=LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	test("parseNumber LONG_MAX",
		!charstring::compare(teststr,
			(testint64==2147483647)
				?"2147483647"
				:"9223372036854775807"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	test("parseNumber LONG_MAX",
		!charstring::compare(teststr,
			(testint64==2147483647)
				?"0000000000000000000000000000002147483647"
				:"0000000000000000000009223372036854775807"));
	testint64=-1*LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	test("parseNumber -LONG_MAX",
		!charstring::compare(teststr,
			(testint64==-2147483647)
				?"-2147483647"
				:"-9223372036854775807"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	test("parseNumber -LONG_MAX",
		!charstring::compare(teststr,
			(testint64==-2147483647)
				?"-000000000000000000000000000002147483647"
				:"-000000000000000000009223372036854775807"));
	delete[] teststr;

	uint32_t	testuint32=4294967295UL;
	teststr=charstring::parseNumber(testuint32);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,"4294967295"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint32,40);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000004294967295"));
	delete[] teststr;

	int32_t		testint32=2147483647;
	teststr=charstring::parseNumber(testint32);
	test("parseNumber 2^31-1",
		!charstring::compare(teststr,"2147483647"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000002147483647"));
	delete[] teststr;
	testint32=-2147483647;
	teststr=charstring::parseNumber(testint32);
	test("parseNumber -2^31-1",
		!charstring::compare(teststr,"-2147483647"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	test("parseNumber -2^32-1",
		!charstring::compare(teststr,
			"-000000000000000000000000000002147483647"));
	delete[] teststr;

	uint16_t	testuint16=65535;
	teststr=charstring::parseNumber(testuint16);
	test("parseNumber 2^16-1",
		!charstring::compare(teststr,"65535"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint16,40);
	test("parseNumber 2^16-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000000000065535"));
	delete[] teststr;

	int16_t		testint16=32767;
	teststr=charstring::parseNumber(testint16);
	test("parseNumber 2^15-1",
		!charstring::compare(teststr,"32767"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	test("parseNumber 2^15-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000000000032767"));
	testint16=-32767;
	teststr=charstring::parseNumber(testint16);
	test("parseNumber -2^15-1",
		!charstring::compare(teststr,"-32767"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	test("parseNumber -2^15-1",
		!charstring::compare(teststr,
			"-000000000000000000000000000000000032767"));
	delete[] teststr;
	stdoutput.printf("\n");
	

	// empty/null/yes/no
	stdoutput.printf("empty/null/yes/no...\n");
	test("NULL",charstring::isNullOrEmpty((const char *)NULL));
	test("empty",charstring::isNullOrEmpty(""));
	test("not null",!charstring::isNullOrEmpty("not null"));
	test("not empty",!charstring::isNullOrEmpty("not empty"));
	test("yes",charstring::isYes("yes"));
	test("Yes",charstring::isYes("Yes"));
	test("YES",charstring::isYes("YES"));
	test("yes,",charstring::isYes("yes,"));
	test("yes ",charstring::isYes("yes "));
	test("yesterday",!charstring::isYes("yesterday"));
	test("Y",charstring::isYes("Y"));
	test("y",charstring::isYes("y"));
	test("yo",!charstring::isYes("yo"));
	test("true",charstring::isYes("true"));
	test("True",charstring::isYes("True"));
	test("TRUE",charstring::isYes("TRUE"));
	test("truest",!charstring::isYes("truest"));
	test("T",charstring::isYes("T"));
	test("t",charstring::isYes("t"));
	test("1",charstring::isYes("1"));
	test("123",!charstring::isYes("123"));
	test("NULL",!charstring::isYes(NULL));
	test("empty",!charstring::isYes(""));
	test("no",charstring::isNo("no"));
	test("No",charstring::isNo("No"));
	test("NO",charstring::isNo("NO"));
	test("nothing",!charstring::isNo("nothing"));
	test("N",charstring::isNo("N"));
	test("n",charstring::isNo("n"));
	test("ne",!charstring::isNo("ne"));
	test("false",charstring::isNo("false"));
	test("False",charstring::isNo("False"));
	test("FALSE",charstring::isNo("FALSE"));
	test("falsest",!charstring::isNo("falsest"));
	test("F",charstring::isNo("F"));
	test("f",charstring::isNo("f"));
	test("0",charstring::isNo("0"));
	test("012",!charstring::isNo("012"));
	test("NULL",!charstring::isNo(NULL));
	test("empty",!charstring::isNo(""));
	stdoutput.printf("\n");
	

	// replace
	stdoutput.printf("replace...\n");
	original="one:-two:-one:-three:-one:-four:-one:-five";
	char	*originalcopy=charstring::duplicate(original);
	charstring::replace(originalcopy,'-','|');
	test("replace char",
		!charstring::compare(originalcopy,
			"one:|two:|one:|three:|one:|four:|one:|five"));
	charstring::replace(originalcopy,'-','|');
	test("replace char (not found)",
		!charstring::compare(originalcopy,
			"one:|two:|one:|three:|one:|four:|one:|five"));
	charstring::replace(originalcopy,":|",'-');
	test("replace set",
		!charstring::compare(originalcopy,
			"one--two--one--three--one--four--one--five"));
	charstring::replace(originalcopy,":|",'-');
	test("replace set (not found)",
		!charstring::compare(originalcopy,
			"one--two--one--three--one--four--one--five"));
	delete[] originalcopy;
	char	*newstr=charstring::replace(original,"one","1");
	test("replace strings (smaller)",
		!charstring::compare(newstr,
			"1:-two:-1:-three:-1:-four:-1:-five"));
	delete[] newstr;
	newstr=charstring::replace(original,"one","oneone");
	test("replace strings (larger)",
		!charstring::compare(newstr,
		"oneone:-two:-oneone:-three:-oneone:-four:-oneone:-five"));
	delete[] newstr;

	newstr=charstring::replace(original,"onee","1");
	test("replace strings (not found)",
		!charstring::compare(newstr,
		"one:-two:-one:-three:-one:-four:-one:-five"));
	delete[] newstr;
	const char * const oldset[]={
		"one","two","three","four","five",NULL
	};
	ssize_t	oldlen[]={
		3,3,5,4,4
	};
	const char * const newset1[]={
		"1","2","3","4","5",NULL
	};
	newstr=charstring::replace(original,oldset,oldlen,newset1);
	test("replace strings set (smaller)",
		!charstring::compare(newstr,
			"1:-2:-1:-3:-1:-4:-1:-5"));
	delete[] newstr;
	const char * const newset2[]={
		"oneone","twotwo","threethree","fourfour","fivefive",NULL
	};
	newstr=charstring::replace(original,oldset,oldlen,newset2);
	test("replace strings set (larger)",
		!charstring::compare(newstr,
			"oneone:-twotwo:-oneone:-threethree:-"
			"oneone:-fourfour:-oneone:-fivefive"));
	delete[] newstr;

	original="0123456789,12345678,234567,3456,45";
	regularexpression	from;
	from.compile("[0-9]*");
	newstr=charstring::replace(original,&from,"numbers",true);
	test("replace regex/from-to (global)",
		!charstring::compare(newstr,
			"numbers,numbers,numbers,numbers,numbers"));
	delete[] newstr;

	newstr=charstring::replace(original,&from,"numbers",false);
	test("replace regex/from-to (first)",
		!charstring::compare(newstr,
			"numbers,12345678,234567,3456,45"));
	delete[] newstr;

	regularexpression	match;
	match.compile("[0-9]*");
	from.compile("5");
	newstr=charstring::replace(original,&match,true,&from,"five",true);
	test("replace regex/match-from-to (larger)",
		!charstring::compare(newstr,
			"01234five6789,1234five678,234five67,34five6,4five"));
	delete[] newstr;

	from.compile("45");
	newstr=charstring::replace(original,&match,true,&from,"fourfive",true);
	test("replace regex/match-from-to (larger)",
		!charstring::compare(newstr,
			"0123fourfive6789,123fourfive678,"
			"23fourfive67,3fourfive6,fourfive"));
	delete[] newstr;

	original="4545454545,45454545,454545,4545,45";
	from.compile("5");
	newstr=charstring::replace(original,&match,true,&from,"five",true);
	test("replace regex/match-from-to (global,global)",
		!charstring::compare(newstr,
			"4five4five4five4five4five,"
			"4five4five4five4five,"
			"4five4five4five,"
			"4five4five,"
			"4five"));
	delete[] newstr;

	newstr=charstring::replace(original,&match,true,&from,"five",false);
	test("replace regex/match-from-to (global,first)",
		!charstring::compare(newstr,
			"4five45454545,"
			"4five454545,"
			"4five4545,"
			"4five45,"
			"4five"));
	delete[] newstr;

	newstr=charstring::replace(original,&match,false,&from,"five",false);
	test("replace regex/match-from-to (first,first)",
		!charstring::compare(newstr,
			"4five45454545,"
			"45454545,"
			"454545,"
			"4545,"
			"45"));
	delete[] newstr;

	newstr=charstring::replace(original,&match,false,&from,"five",true);
	test("replace regex/match-from-to (first,global)",
		!charstring::compare(newstr,
			"4five4five4five4five4five,"
			"45454545,"
			"454545,"
			"4545,"
			"45"));
	delete[] newstr;

	stdoutput.printf("\n");


	// hosttonet/nettohost (why are these here?)
	/*unsigned char	v8=0xC0;
	uint16_t	v16=filedescriptor::hostToNet(
						(uint16_t)0xC0C0);
	uint32_t	v32=filedescriptor::hostToNet(
						(uint32_t)0xC0C0C0C0);
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	v64=filedescriptor::hostToNet(
					(uint64_t)0xC0C0C0C0C0C0C0C0ULL);
	#endif
	char	sv8=-1*0x01;
	int16_t	sv16=filedescriptor::hostToNet((uint16_t)
						(int16_t)(-1*0x0101));
	int32_t	sv32=filedescriptor::hostToNet((uint32_t)
						(int32_t)(-1*0x01010101));
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	int64_t	sv64=filedescriptor::hostToNet((uint64_t)
					(int64_t)(-1*0x0101010101010101LL));
	#endif
	unsigned char	v8s[]={0xC0,0xC0,0xC0,0xC0};
	stdoutput.printBits(v8);
	stdoutput.printf("\n");
	stdoutput.printBits(v16);
	stdoutput.printf("\n");
	stdoutput.printBits(v32);
	stdoutput.printf("\n");
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	stdoutput.printBits(v64);
	stdoutput.printf("\n");
	#endif
	stdoutput.printBits(sv8);
	stdoutput.printf("\n");
	stdoutput.printBits(sv16);
	stdoutput.printf("\n");
	stdoutput.printBits(sv32);
	stdoutput.printf("\n");
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	stdoutput.printBits(sv64);
	stdoutput.printf("\n");
	#endif
	stdoutput.printBits(v8s,sizeof(v8s));
	stdoutput.printf("\n");*/
}
