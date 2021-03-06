// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	char	*osname=sys::getOperatingSystemName();
	const char	*groupname="bin";
	if (!charstring::compare(osname,"Windows")) {
		groupname="None";
	} else if (!charstring::compare(osname,"Haiku")) {
		groupname="root";
	}
	delete[] osname;


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
	test("sid",
		!charstring::compare(grent.getSidString(),"S-1-5-21-",9) &&
		!charstring::compare(grent.getSidString()+
			charstring::length(grent.getSidString())-4,"-513"));
#else
	test("group id",grent.getGroupId()<10);
	// the bin group has members on some systems
	//test("members",!grent.getMembers());
	test("sid",charstring::toInteger(grent.getSidString())<10);
#endif
	stdoutput.printf("\n");

	gid_t	id=grent.getGroupId();
	stdoutput.printf("%d:\n",id);
	test("initialize",grent.initialize(id));
	test("name",!charstring::compare(grent.getName(),groupname));
#ifdef _WIN32
	test("group id",grent.getGroupId()==0);
	test("members",grent.getMembers() && grent.getMembers()[0]);
	test("sid",
		!charstring::compare(grent.getSidString(),"S-1-5-21-",9) &&
		!charstring::compare(grent.getSidString()+
			charstring::length(grent.getSidString())-4,"-513"));
#else
	test("group id",grent.getGroupId()<10);
	// the bin group has members on some systems
	//test("members",!grent.getMembers());
	test("sid",charstring::toInteger(grent.getSidString())<10);
#endif
	stdoutput.printf("\n");

	stdoutput.printf("invalid group name\n");
	test("initialize",!grent.initialize("invalid group name"));
	test("name",!grent.getName());
	test("group id",grent.getGroupId()==(gid_t)-1);
	test("members",!grent.getMembers());
	test("sid",!grent.getSidString());
	stdoutput.printf("\n");

	// invalid group id
	stdoutput.printf("-1:\n");
	test("initialize",!grent.initialize((gid_t)-1));
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
