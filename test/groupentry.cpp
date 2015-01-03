// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/stdio.h>

#ifdef _WIN32
const char	*groupname="None";
#else
const char	*groupname="bin";
#endif

void print(groupentry *grent) {

	stdoutput.printf("	Name:		%s\n",grent->getName());
	stdoutput.printf("	Group Id:	%d\n",grent->getGroupId());
	stdoutput.printf("	Members:\n");
	int i;
	for (i=0; grent->getMembers() && grent->getMembers()[i]; i++) {
		stdoutput.printf("		%s\n",grent->getMembers()[i]);
	}
	stdoutput.printf("	SID:		%s\n",grent->getSid());
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// get the group entry for the groupname
	groupentry	grent;
	stdoutput.printf("uninitialized:\n");
	print(&grent);

	// get the group entry for the groupname
	grent.initialize(groupname);
	stdoutput.printf("groupname: %s...\n",groupname);
	print(&grent);

	// get the group entry for group id of the group we just looked up
	gid_t	id=grent.getGroupId();
	grent.initialize(id);
	stdoutput.printf("groupid: %d...\n",id);
	print(&grent);

	// invalid group
	grent.initialize("invalidgroupname");
	stdoutput.printf("groupname: invalidgroupname...\n");
	print(&grent);

	// invalid group id
	id=grent.getGroupId();
	grent.initialize(id);
	stdoutput.printf("groupid: %d (invalid)...\n",id);
	print(&grent);

	// null-safety
	grent.initialize((const char *)NULL);
	stdoutput.printf("NULL:\n");
	print(&grent);
}
