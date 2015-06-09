#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// comparing string...
	const char * const strings[]={
		"hello","HELLO","hello there","HELLO THERE",NULL
	};

	stdoutput.write("direct comparison...\n");
	for (const char * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%s\"  %s?\n",*s,
			(!charstring::compare("hello",*s))?"yes":"no");
		
	}
	stdoutput.write('\n');

	stdoutput.write("only first 5 bytes...\n");
	for (const char * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",*s,
			(!charstring::compare("hello",*s,5))?"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const char * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",*s,
			(!charstring::compareIgnoringCase("hello",*s,5))?
								"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case, only first 5 bytes...\n");
	for (const char * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",*s,
			(!charstring::compareIgnoringCase("hello",*s,5))?
								"yes":"no");
	}
	stdoutput.write('\n');



	// member of a set...
	const char * const greetings[]={
		"hello","hi","good morning",NULL
	};
	const char * const lowercaseexpressions[]={
		"hello","hi","bye","goodbye",NULL
	};
	const char * const uppercaseexpressions[]={
		"HELLO","HI","BYE","GOODBYE",NULL
	};

	stdoutput.write("considering case...\n");
	for (const char * const *le=lowercaseexpressions; *le; le++) {
		stdoutput.printf("  is \"%s\" a greeting?  %s\n",*le,
			(charstring::inSet(*le,greetings))?"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const char * const *ue=uppercaseexpressions; *ue; ue++) {
		stdoutput.printf("  is \"%s\" a greeting?  %s\n",*ue,
			(charstring::inSetIgnoringCase(*ue,greetings))?
								"yes":"no");
	}
	stdoutput.write('\n');


	// does one string contain another?
	const char	phrase[]="the quick brown fox jumped over the lazy dog";
	const char * const lowercasewords[]={
		"quick","brown","fox","lazy","dog","hello","goodbye",NULL
	};
	const char * const uppercasewords[]={
		"QUICK","BROWN","FOX","LAZY","DOG","HELLO","GOODBYE",NULL
	};
	
	stdoutput.write("considering case...\n");
	for (const char * const *lw=lowercasewords; *lw; lw++) {
		stdoutput.printf("  does \"%s\" contain \"%s\"?  %s\n",
			phrase,*lw,
			(charstring::contains(phrase,*lw))?"yes":"no");
	}
	stdoutput.write('\n');
	
	stdoutput.write("ignoring case...\n");
	for (const char * const *uw=uppercasewords; *uw; uw++) {
		stdoutput.printf("  does \"%s\" contain \"%s\"?  %s\n",
			phrase,*uw,
			(charstring::contains(phrase,*uw))?"yes":"no");
	}
	stdoutput.write('\n');
}
