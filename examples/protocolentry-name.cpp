#include <rudiments/protocolentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// get the protocol entry for the "tcp" protocol
	protocolentry	pe;
	pe.initialize("tcp");

	// print out details
	stdoutput.printf("	Name:		%s\n",pe.getName());
	stdoutput.printf("	Alias list:\n");
	for (uint32_t i=0; pe.getAliasList() && pe.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",pe.getAliasList()[i]);
	}
	stdoutput.printf("	Number:		%d\n",pe.getNumber());
	stdoutput.printf("\n");
}
