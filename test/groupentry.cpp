// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

#ifdef _WIN32
const char	*groupname="None";
#else
const char	*groupname="bin";
#endif

int main(int argc, const char **argv) {

	header("groupentry");

	groupentry	grent;

	stdoutput.printf("uninitialized:\n");
	test("name",!grent.getName());
	test("group id",grent.getGroupId()==(gid_t)-1);
	test("members",!grent.getMembers());
	test("sid",!grent.getSidString());
	stdoutput.printf("\n");

	stdoutput.printf("%s:\n",groupname);
	test("initialize",grent.initialize(groupname));
	test("name",!charstring::compare(grent.getName(),groupname));
#ifdef _WIN32
	test("group id",grent.getGroupId()==0);
	test("members",grent.getMembers() && grent.getMembers()[0]);
	test("sid",!charstring::compare(grent.getSidString(),
			"S-1-5-21-1873234618-1269098444-2064074030-513"));
#else
	test("group id",grent.getGroupId()==1);
	test("members",grent.getMembers() && !grent.getMembers()[0]);
	test("sid",!charstring::compare(grent.getSidString(),"1"));
#endif
	stdoutput.printf("\n");

	gid_t	id=grent.getGroupId();
	stdoutput.printf("%d:\n",id);
	test("initialize",grent.initialize(id));
	test("name",!charstring::compare(grent.getName(),groupname));
#ifdef _WIN32
	test("group id",grent.getGroupId()==0);
	test("members",grent.getMembers() && grent.getMembers()[0]);
	test("sid",!charstring::compare(grent.getSidString(),
			"S-1-5-21-1873234618-1269098444-2064074030-513"));
#else
	test("group id",grent.getGroupId()==1);
	test("members",grent.getMembers() && !grent.getMembers()[0]);
	test("sid",!charstring::compare(grent.getSidString(),"1"));
#endif
	stdoutput.printf("\n");

	stdoutput.printf("invalid group name\n");
	grent.initialize("invalid group name");
	test("initialize",!grent.initialize("invalid group name"));
	test("name",!grent.getName());
	test("group id",grent.getGroupId()==(gid_t)-1);
	test("members",!grent.getMembers());
	test("sid",!grent.getSidString());
	stdoutput.printf("\n");

	// invalid group id
	id=grent.getGroupId();
	stdoutput.printf("%d:\n",id);
	grent.initialize(id);
	test("initialize",!grent.initialize("invalid group name"));
	test("name",!grent.getName());
	test("group id",grent.getGroupId()==(gid_t)-1);
	test("members",!grent.getMembers());
	test("sid",!grent.getSidString());
	stdoutput.printf("\n");

	// null-safety
	stdoutput.printf("NULL\n");
	test("initialize",!grent.initialize((const char *)NULL));
	test("name",!grent.getName());
	test("group id",grent.getGroupId()==(gid_t)-1);
	test("members",!grent.getMembers());
	test("sid",!grent.getSidString());
	stdoutput.printf("\n");
}
