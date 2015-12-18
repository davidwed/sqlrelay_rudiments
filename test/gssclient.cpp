#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/commandline.h>
#include <rudiments/gss.h>

#include "gssmisc.h"

static void usage() {
	stdoutput.printf("Usage: gss-client "
			"-host host -service service -message msg "
			"[-user user] [-pass pw] "
			"[-ccount count] [-mcount count] [-dcount count]"
			"[-mech mechanism] "
			"[-deleg] [-seq] [-noreplay] [-nomutual] "
			"[-verbose]\n");
}

int main(int argc, const char **argv) {

	// process command line arguments...

	commandline	cmdl(argc,argv);

	// host, service, message
	const char	*host=cmdl.getValue("host");
	const char	*service=cmdl.getValue("service");
	const char	*msg=cmdl.getValue("message");
	if (!host || !service || !msg) {
		usage();
		process::exit(1);
	}

	// user, password
	const char	*username=cmdl.getValue("user");
	const char	*password=cmdl.getValue("pass");

	// connection and message count
	int64_t	ccount=charstring::toInteger(cmdl.getValue("ccount"));
	if (ccount<0) {
		usage();
		process::exit(1);
	} else if (!ccount) {
		ccount=1;
	}
	int64_t	mcount=charstring::toInteger(cmdl.getValue("mcount"));
	if (mcount<0) {
		usage();
		process::exit(1);
	} else if (!mcount) {
		mcount=1;
	}
	int64_t	dcount=charstring::toInteger(cmdl.getValue("dcount"));
	if (dcount<0) {
		usage();
		process::exit(1);
	} else if (!dcount) {
		dcount=1;
	}


	// mechanism
	const char	*mechanism=cmdl.getValue("mech");

	// flags
	uint32_t	flags=0 |
			((cmdl.found("deleg"))?GSS_C_DELEG_FLAG:0) |
			((cmdl.found("seq"))?GSS_C_SEQUENCE_FLAG:0) |
			((cmdl.found("noreplay"))?0:GSS_C_REPLAY_FLAG) |
			((cmdl.found("nomutual"))?0:GSS_C_MUTUAL_FLAG);

	// verbose
	bool	verbose=cmdl.found("verbose");


	// duplicate the message the
	// specified number of times...
	bytebuffer	msgbuf;
	for (int64_t i=0; i<dcount; i++) {
		msgbuf.append(msg)->append(' ');
	}
	

	// print available mechs
	if (verbose) {
		displayAvailableMechanisms();
	}

	// configure mechanisms
	gssmechanism		gmech;
	gmech.initialize(mechanism);

	// acquire credentials
	gsscredentials	gcred;
	gcred.addDesiredMechanism(&gmech);
	if (!charstring::isNullOrEmpty(username) &&
		!gcred.acquireUserName(username,password)) {
		stdoutput.printf("acquireUserName():\n");
		stdoutput.printf("%s\n",gcred.getStatus());
		stdoutput.printf("}\n");
		process::exit(1);
	}

	// print information about the credentials
	if (verbose) {
		displayCredentials(&gcred);
	}

	// configure security context
	gsscontext	gctx;
	gctx.setDesiredMechanism(&gmech);
	gctx.setDesiredFlags(flags);
	gctx.setService("gssserver");

	// configure socket
	inetsocketclient	fd;
	fd.setWriteBufferSize(8192);
	fd.setReadBufferSize(8192);
	fd.setGSSContext(&gctx);

	// loop, having sessions with server
	for (int64_t i=0; i<ccount; i++) {

		// connect (will also initiate context)
		if (!fd.connect(host,4444,-1,-1,1,0)) {
			stdoutput.printf("connect():\n");
			stdoutput.printf("%s\n",gctx.getStatus());
			continue;
		}

		// print information about the context
		if (verbose) {
			displayContext(&gctx);
		}

		stdoutput.printf("serverSession {\n");

		// write message to the server, the specified number of times...
		for (int64_t j=0; j<mcount; j++) {

			displayData("Message",
					msgbuf.getBuffer(),
					msgbuf.getSize());

			if (fd.write((uint64_t)
					msgbuf.getSize())!=
					sizeof(uint64_t)) {
				stdoutput.printf("\n  write() size failed\n");
				continue;
			}
			if (fd.write(msgbuf.getBuffer(),
					msgbuf.getSize())!=
					(ssize_t)msgbuf.getSize()) {
				stdoutput.printf("\n  write() msg failed\n");
				continue;
			}
			fd.flushWriteBuffer(-1,-1);
		}

		stdoutput.printf("}\n");
	}

	process::exit(0);
}
