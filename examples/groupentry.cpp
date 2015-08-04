#include <rudiments/groupentry.h>
#include <rudiments/stdio.h>

#ifdef _WIN32
const char	*groupname="None";
#else
const char	*groupname="bin";
#endif

int main(int argc, const char **argv) {

	// get the group entry
	groupentry	grent;
	grent.initialize(groupname);

	// print out details
	stdoutput.printf("	Name:		%s\n",grent.getName());
	stdoutput.printf("	Group Id:	%d\n",grent.getGroupId());
	stdoutput.printf("	Members:\n");
	for (uint32_t i=0; grent.getMembers() && grent.getMembers()[i]; i++) {
		stdoutput.printf("		%s\n",grent.getMembers()[i]);
	}
	stdoutput.printf("	SID:		%s\n",grent.getSidString());
	stdoutput.printf("\n");
}
