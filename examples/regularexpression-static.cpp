#include <rudiments/regularexpression.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char	*string="void f(int a, bool b) { exit(0); }";
	const char	*pattern="(void|int|bool).*f\\(.*\\) { .* }";

	const char	*matches;
	if (regularexpression::match(string,pattern)) {
		matches="matches";
	} else {
		matches="doesn't match";
	}

	stdoutput.printf("%s\n  %s\n%s\n",string,matches,pattern);
}
