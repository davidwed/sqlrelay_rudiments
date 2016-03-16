// Copyright (c) 2015-2016  David Muse
// See the file COPYING for more information

#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/commandline.h>
#include <rudiments/error.h>
#include <rudiments/gss.h>

static void printFlags(uint32_t flags) {
	#ifdef GSS_C_DELEG_FLAG
		if (flags&GSS_C_DELEG_FLAG) {
			stdoutput.printf(
				"    GSS_C_DELEG_FLAG\n");
		}
	#endif
	#ifdef GSS_C_MUTUAL_FLAG
		if (flags&GSS_C_MUTUAL_FLAG) {
			stdoutput.printf(
				"    GSS_C_MUTUAL_FLAG\n");
		}
	#endif
	#ifdef GSS_C_REPLAY_FLAG
		if (flags&GSS_C_REPLAY_FLAG) {
			stdoutput.printf(
				"    GSS_C_REPLAY_FLAG\n");
		}
	#endif
	#ifdef GSS_C_SEQUENCE_FLAG
		if (flags&GSS_C_SEQUENCE_FLAG) {
			stdoutput.printf(
			"    GSS_C_SEQUENCE_FLAG\n");
		}
	#endif
	#ifdef GSS_C_CONF_FLAG
		if (flags&GSS_C_CONF_FLAG) {
			stdoutput.printf(
				"    GSS_C_CONF_FLAG\n");
		}
	#endif
	#if !defined(RUDIMENTS_HAS_SSPI)
		#ifdef GSS_C_INTEG_FLAG
			if (flags&GSS_C_INTEG_FLAG) {
				stdoutput.printf(
					"    GSS_C_INTEG_FLAG\n");
			}
		#endif
		#ifdef GSS_C_ANON_FLAG
			if (flags&GSS_C_ANON_FLAG) {
				stdoutput.printf(
					"    GSS_C_ANON_FLAG\n");
			}
		#endif
		#ifdef GSS_C_PROT_READY_FLAG
			if (flags&GSS_C_PROT_READY_FLAG) {
				stdoutput.printf(
					"    GSS_C_PROT_READY_FLAG\n");
			}
		#endif
		#ifdef GSS_C_TRANS_FLAG
			if (flags&GSS_C_TRANS_FLAG) {
				stdoutput.printf(
					"    GSS_C_TRANS_FLAG\n");
			}
		#endif
		#ifdef GSS_C_DELEG_POLICY_FLAG
			if (flags&GSS_C_DELEG_POLICY_FLAG) {
				stdoutput.printf(
					"    GSS_C_DELEG_POLICY_FLAG\n");
			}
		#endif
	#endif
}

static void usage() {
	stdoutput.printf("Usage: gss-client "
			"[-host host] [-port port] [-service service] "
			"[-message msg] "
			"[-user user] "
			"[-mech mechanism] [-flags flags] "
			"[-ccount count] [-mcount count] [-dcount count] "
			"[-verbose]\n");
}

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);
	if (cmdl.found("help")) {
		usage();
		process::exit(1);
	}
	const char	*host="127.0.0.1";
	if (cmdl.found("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*service="gssserver";
	if (cmdl.found("service")) {
		service=cmdl.getValue("service");
	}
	const char	*msg="hello";
	if (cmdl.found("message")) {
		msg=cmdl.getValue("message");
	}
	const char	*username=cmdl.getValue("user");
	const char	*mechanism=cmdl.getValue("mech");
	const char	*flags="GSS_C_REPLAY_FLAG,GSS_C_MUTUAL_FLAG";
	if (cmdl.found("flags")) {
		flags=cmdl.getValue("flags");
	}
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
	bool	verbose=cmdl.found("verbose");


	// duplicate the message the
	// specified number of times...
	bytebuffer	msgbuf;
	for (int64_t i=0; i<dcount; i++) {
		msgbuf.append(msg)->append(' ');
	}

	// print available mechs
	if (verbose) {
		gss	g;
		const char * const	*mechs=g.getAvailableMechanisms();
		stdoutput.printf("Available Mechanisms {\n");
		for (const char * const *m=mechs; *m; m++) {
			stdoutput.printf("  %s\n",*m);
		}
		stdoutput.printf("}\n");
	}

	// configure mechanisms
	gssmechanism		mech;
	mech.initialize(mechanism);

	// configure the security context
	gsscontext	ctx;
	ctx.setDesiredMechanism(&mech);
	ctx.setDesiredFlags(flags);
	ctx.setService(service);

	// acquire credentials, if necessary
	gsscredentials	cred;
	if (!charstring::isNullOrEmpty(username)) {

		cred.addDesiredMechanism(&mech);
		if (!cred.acquireForUser(username)) {
			stdoutput.printf("acquireForUser():\n");
			stdoutput.printf("%s\n",cred.getStatus());
			stdoutput.printf("}\n");
			process::exit(1);
		}

		// print information about the credentials
		if (verbose) {
			stdoutput.write("Credentials {\n");
			stdoutput.printf("  name: %s\n",cred.getName());
			stdoutput.printf("  desired lifetime: %d\n",
					cred.getDesiredLifetime());
			stdoutput.printf("  actual lifetime: %d\n",
					cred.getActualLifetime());
			stdoutput.write("  desired mechanisms:\n");
			for (uint64_t i=0;
				 i<cred.getDesiredMechanismCount(); i++) {
				gssmechanism	*m=cred.getDesiredMechanism(i);
				if (m) {
					stdoutput.printf("    %s\n",
							m->getString());
				}
			}
			stdoutput.write("  actual mechanisms:\n");
			for (uint64_t i=0;
				i<cred.getActualMechanismCount(); i++) {
				gssmechanism	*m=cred.getActualMechanism(i);
				if (m) {
					stdoutput.printf("    %s\n",
							m->getString());
				}
			}
			stdoutput.write("}\n");
		}

		// attach the credentials to the context
		ctx.setCredentials(&cred);
	}

	// create an inet socket client
	inetsocketclient	fd;
	fd.setWriteBufferSize(65536);
	fd.setReadBufferSize(65536);

	// attach the security context
	fd.setSecurityContext(&ctx);

	// loop, having sessions with the server
	for (int64_t i=0; i<ccount; i++) {

		// connect
		if (fd.connect(host,port,-1,-1,0,1)!=RESULT_SUCCESS) {
			if (error::getErrorNumber()) {
				stdoutput.printf("connect failed (1): %s\n",
							error::getErrorString());
			} else {
				stdoutput.printf("connect failed (2): %s\n",
						ctx.getMechanismMinorStatus());
			}
			continue;
		}

		// print information about the context
		if (verbose) {
			stdoutput.write("Context {\n");
			stdoutput.printf("  file descriptor: %d\n",
				(ctx.getFileDescriptor())?
					ctx.getFileDescriptor()->
						getFileDescriptor():-1);
			stdoutput.printf("  desired lifetime: %d\n",
					ctx.getDesiredLifetime());
			stdoutput.printf("  actual lifetime: %d\n",
					ctx.getActualLifetime());
			stdoutput.printf("  desired mechanism: %s\n",
				(ctx.getDesiredMechanism() &&
					ctx.getDesiredMechanism()->
					getString())?
				ctx.getDesiredMechanism()->getString():
				"(none)");
			stdoutput.printf("  actual mechanism: %s\n",
				ctx.getActualMechanism()->getString());
			stdoutput.write("  desired flags:\n");
			printFlags(ctx.getDesiredFlags());
			stdoutput.write("  actual flags:\n");
			printFlags(ctx.getActualFlags());
			stdoutput.printf("  service: %s\n",
				(ctx.getService())?
					ctx.getService():"(none)");
			stdoutput.printf("  initiator: %s\n",
				(ctx.getInitiator())?
					ctx.getInitiator():"(none)");
			stdoutput.printf("  acceptor: %s\n",
				(ctx.getAcceptor())?
					ctx.getAcceptor():"(none)");
			stdoutput.printf("  is initiator: %s\n",
				(ctx.getIsInitiator())?"yes":"no");
			stdoutput.printf("  is open: %s\n",
				(ctx.getIsOpen())?"yes":"no");
			stdoutput.write("}\n");
		}

		stdoutput.printf("serverSession {\n");

		// write the message to the server,
		// the specified number of times...
		for (int64_t j=0; j<mcount; j++) {

			// write size
			ssize_t	sizewritten=fd.write((uint64_t)
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					break;
				}
			} else if (sizewritten!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  write() size failed (3): %s\n",
					ctx.getMechanismMinorStatus());
				break;
			}

			// write message
			sizewritten=fd.write(msgbuf.getBuffer(),
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() msg failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() msg failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() msg failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					break;
				}
			} else if (sizewritten!=(ssize_t)msgbuf.getSize()) {
				stdoutput.printf(
					"  write() msg failed (3): %s\n",
					ctx.getMechanismMinorStatus());
				break;
			}

			// flush write buffer
			if (!fd.flushWriteBuffer(-1,-1)) {
				stdoutput.printf("flushWriteBuffer() failed\n");
				break;
			}

			stdoutput.printf("\n  Sent message... "
					"(size=%d):\n  ",msgbuf.getSize());
			stdoutput.safePrint(msgbuf.getBuffer(),
				(msgbuf.getSize()<=300)?msgbuf.getSize():300);
			if (msgbuf.getSize()>300) {
				stdoutput.write("...");
			}
			stdoutput.write('\n');
			stdoutput.printf("\n  Receiving response...");

			// read size
			uint64_t	msgsize;
			ssize_t	sizeread=fd.read(&msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  read() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					break;
				}
			} else if (sizeread!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  read() size failed (3): %s\n",
					ctx.getMechanismMinorStatus());
				break;
			}

			// read message
			unsigned char	*msg=new unsigned char[msgsize];
			sizeread=fd.read(msg,msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() msg failed (0): "
						"eof\n");
					delete[] msg;
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() msg failed (1): "
						"%s\n",
						error::getErrorString());
					delete[] msg;
					break;
				} else {
					stdoutput.printf(
						"  read() msg failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					delete[] msg;
					break;
				}
			} else if (sizeread!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  read() msg failed (3): %s\n",
					ctx.getMechanismMinorStatus());
				delete[] msg;
				break;
			}

			stdoutput.printf(" success\n");

			delete[] msg;
		}

		stdoutput.printf("}\n");

		// close the connection to the server
		fd.close();
	}

	process::exit(0);
}
