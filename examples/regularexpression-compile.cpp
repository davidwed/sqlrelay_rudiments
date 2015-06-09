#include <rudiments/regularexpression.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char	*pattern="(void|int|bool).*f\\(.*\\) { .* }";
	const char * const strings[]={
		"class t { public: int f(int a); void f(int a, bool b); };",
		"void f(int a, bool b) { exit(0); }",
		"int f(int a) { printf(\"hello\\n\"); }",
		"struct m { int a; int b; int c; };",
		NULL
	};

	regularexpression	re;
	re.compile(pattern);
	re.study();

	for (const char * const *s=strings; *s; s++) {
	
		const char	*matches;
		if (re.match(*s,pattern)) {
			matches="does match";
		} else {
			matches="doesn't match";
		}

		stdoutput.printf("%s\n  %s\n%s\n\n",*s,matches,pattern);
	}

	process::exit(0);
}
