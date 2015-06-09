#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// conversion of numbers to strings...
	char	*intstr=charstring::parseNumber((uint64_t)12345);
	char	*floatstr=charstring::parseNumber((float)12.345,5,3);

	stdoutput.printf("numbers as strings: %s, %s\n",intstr,floatstr);
	stdoutput.write('\n');

	delete[] intstr;
	delete[] floatstr;



	// conversion of strings to numbers...
	int64_t		intnum=charstring::toInteger("12345");
	uint64_t	uintnum=charstring::toUnsignedInteger("12345");
	long double	floatnum=charstring::toFloat("12.345");

	stdoutput.printf("strings as numbers: %lld, %lld, %5.3Lf\n",
						intnum,uintnum,floatnum);
	stdoutput.write('\n');



	// identification of numeric strings...
	const char * const numbers[]={
		"1","-1","1.1","-1.1","one","hello",NULL
	};
	
	for (const char * const *n=numbers; *n; n++) {
		stdoutput.printf("%s %s a number\n",*n,
				(charstring::isNumber(*n))?"is":"is not");
		stdoutput.printf("%s %s an integer\n",*n,
				(charstring::isInteger(*n))?"is":"is not");
	}
	stdoutput.write('\n');



	// integer lengths
	uint64_t	integers[]={
		1,23,456,7890,12345,678901,0
	};

	for (uint64_t *i=integers; *i; i++) {
		stdoutput.printf("it would take %d bytes to store "
					"%lld as a string\n",
					charstring::integerLength(*i),*i);
	}
	stdoutput.write('\n');



	// dollar amounts
	const char	dollarstr[]="$123.45";
	int64_t	pennies=charstring::convertAmount(dollarstr);
	char	*dollars=charstring::convertAmount(pennies);
	stdoutput.printf("%s as pennies: %lld\n",dollarstr,pennies);
	stdoutput.printf("%lld pennies as dollars: %s\n",pennies,dollars);
}
