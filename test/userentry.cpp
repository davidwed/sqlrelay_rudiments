// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/userentry.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	char	*osname=sys::getOperatingSystemName();
	const char	*username="bin";
	if (!charstring::compare(osname,"Haiku")) {
		username="user";
	}


	header("userentry");

	// uninitialized
	userentry	uent;
	stdoutput.printf("uninitialized...\n");
	test("name",!uent.getName());
	test("password",!uent.getPassword());
	test("user id",uent.getUserId()==(uid_t)-1);
	test("primary group id",uent.getPrimaryGroupId()==(gid_t)-1);
	test("real name",!uent.getRealName());
	test("home dir",!uent.getHomeDirectory());
	test("shell",!uent.getShell());
	test("encrypted password",!uent.getEncryptedPassword());
	test("last change date",uent.getLastChangeDate()==-1);
	test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
	test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
	test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
	test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
	test("expiration date",
			uent.getExpirationDate()==-1);
	test("flag",uent.getFlag()==-1);
	test("sid",!uent.getSidString());
	stdoutput.printf("\n");

	// get the user entry for "bin"
	stdoutput.printf("username: %s...\n",username);
	uent.initialize(username);
	test("name",!charstring::compare(uent.getName(),username));
#ifndef _WIN32
	test("password",uent.getPassword());
	if (!charstring::compare(osname,"Haiku")) {
		test("user id",(int64_t)uent.getUserId()==0);
		test("primary group id",(int64_t)uent.getPrimaryGroupId()==0);
	} else {
		test("user id",(int64_t)uent.getUserId()>0 &&
				(int64_t)uent.getUserId()<65536);
		test("primary group id",
				(int64_t)uent.getPrimaryGroupId()>0 &&
				(int64_t)uent.getPrimaryGroupId()<65536);
	}
#endif
	test("real name",uent.getRealName());
	test("home dir",uent.getHomeDirectory());
#ifdef _WIN32
	test("shell",!uent.getShell());
#else
	test("shell",uent.getShell());
#endif
	if (!process::getUserId()) {
#ifndef _WIN32
		test("encrypted password",uent.getEncryptedPassword());
#endif
		// no good way to validate these, they could legitimately
		// be any number, including -1
		/*test("last change date",uent.getLastChangeDate()==-1);
		test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
		test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
		test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
		test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
		test("expiration date",
			uent.getExpirationDate()==-1);
		test("flag",uent.getFlag()==-1);*/
	}
#ifdef _WIN32
	test("sid",uent.getSidString() &&
			charstring::compare(uent.getSidString(),"-1"));
#else
	char	*uidstr=charstring::parseNumber((uint64_t)uent.getUserId());
	test("sid",uent.getSidString() &&
			!charstring::compare(uidstr,uent.getSidString()));
	delete[] uidstr;
#endif
	stdoutput.printf("\n");

	// get the user entry for user id of the user we just looked up
	uid_t	id=uent.getUserId();
	stdoutput.printf("userid: %d...\n",id);
	uent.initialize(id);
	test("name",!charstring::compare(uent.getName(),username));
#ifndef _WIN32
	test("password",uent.getPassword());
	if (!charstring::compare(osname,"Haiku")) {
		test("user id",(int64_t)uent.getUserId()==0);
		test("primary group id",(int64_t)uent.getPrimaryGroupId()==0);
	} else {
		test("user id",(int64_t)uent.getUserId()>0 &&
				(int64_t)uent.getUserId()<65536);
		test("primary group id",
				(int64_t)uent.getPrimaryGroupId()>0 &&
				(int64_t)uent.getPrimaryGroupId()<65536);
	}
#endif
	test("real name",uent.getRealName());
	test("home dir",uent.getHomeDirectory());
#ifdef _WIN32
	test("shell",!uent.getShell());
#else
	test("shell",uent.getShell());
#endif
	if (!process::getUserId()) {
#ifndef _WIN32
		test("encrypted password",uent.getEncryptedPassword());
#endif
		// no good way to validate these, they could legitimately
		// be any number, including -1
		/*test("last change date",uent.getLastChangeDate()==-1);
		test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
		test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
		test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
		test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
		test("expiration date",
			uent.getExpirationDate()==-1);
		test("flag",uent.getFlag()==-1);*/
	}
#ifdef _WIN32
	test("sid",uent.getSidString() &&
			charstring::compare(uent.getSidString(),"-1"));
#else
	uidstr=charstring::parseNumber((uint64_t)uent.getUserId());
	test("sid",uent.getSidString() &&
			!charstring::compare(uidstr,uent.getSidString()));
	delete[] uidstr;
#endif
	stdoutput.printf("\n");

	// invalid user
	stdoutput.printf("username: invalidusername...\n");
	uent.initialize("invalidusername");
	test("name",!uent.getName());
	test("password",!uent.getPassword());
	test("user id",uent.getUserId()==(uid_t)-1);
	test("primary group id",uent.getPrimaryGroupId()==(gid_t)-1);
	test("real name",!uent.getRealName());
	test("home dir",!uent.getHomeDirectory());
	test("shell",!uent.getShell());
	test("encrypted password",!uent.getEncryptedPassword());
	test("last change date",uent.getLastChangeDate()==-1);
	test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
	test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
	test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
	test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
	test("expiration date",
			uent.getExpirationDate()==-1);
	test("flag",uent.getFlag()==-1);
	stdoutput.printf("\n");

	// invalid user id
	id=uent.getUserId();
	stdoutput.printf("userid: %d (invalid)...\n",id);
	uent.initialize(id);
	test("name",!uent.getName());
	test("password",!uent.getPassword());
	test("user id",uent.getUserId()==(uid_t)-1);
	test("primary group id",uent.getPrimaryGroupId()==(gid_t)-1);
	test("real name",!uent.getRealName());
	test("home dir",!uent.getHomeDirectory());
	test("shell",!uent.getShell());
	test("encrypted password",!uent.getEncryptedPassword());
	test("last change date",uent.getLastChangeDate()==-1);
	test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
	test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
	test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
	test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
	test("expiration date",
			uent.getExpirationDate()==-1);
	test("flag",uent.getFlag()==-1);
	test("sid",!uent.getSidString());
	stdoutput.printf("\n");

	// null-safety
	stdoutput.printf("NULL...\n");
	uent.initialize((const char *)NULL);
	test("name",!uent.getName());
	test("password",!uent.getPassword());
	test("user id",uent.getUserId()==(uid_t)-1);
	test("primary group id",uent.getPrimaryGroupId()==(gid_t)-1);
	test("real name",!uent.getRealName());
	test("home dir",!uent.getHomeDirectory());
	test("shell",!uent.getShell());
	test("encrypted password",!uent.getEncryptedPassword());
	test("last change date",uent.getLastChangeDate()==-1);
	test("days before change allowed",
			uent.getDaysBeforeChangeAllowed()==-1);
	test("days before change required",
			uent.getDaysBeforeChangeRequired()==-1);
	test("days before expiration warning",
			uent.getDaysBeforeExpirationWarning()==-1);
	test("days of inactivity allowed",
			uent.getDaysOfInactivityAllowed()==-1);
	test("expiration date",
			uent.getExpirationDate()==-1);
	test("flag",uent.getFlag()==-1);
	test("sid",!uent.getSidString());
	stdoutput.printf("\n");

	delete[] osname;
}
