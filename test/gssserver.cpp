#include <rudiments/inetsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/character.h>
#include <rudiments/commandline.h>
#include <rudiments/environment.h>
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

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);

	if (cmdl.found("help")) {
		stdoutput.printf("gssserver [-port port] [-keytab keytab] [-service service] [-verbose]\n");
		process::exit(0);
	}

	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*keytab=cmdl.getValue("keytab");
	if (charstring::isNullOrEmpty(keytab)) {
		#ifndef WIN32
		if (process::getUserId()) {
			stdoutput.printf("Warning! Since you're using the "
					"system keytab,\n         "
					"this program should probably be "
					"run as root.\n\n");
		}
		#endif
	}
	const char	*service="gssserver";
	if (cmdl.found("service")) {
		service=cmdl.getValue("service");
	}
	bool		verbose=cmdl.found("verbose");

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

	gsscredentials	cred;

	// set which keytab to use
	if (!charstring::isNullOrEmpty(keytab) && !cred.setKeytab(keytab)) {
		stdoutput.printf("failed to configure keytab\n");
		process::exit(1);
	}

	// acquire the credentials for this service
	if (!cred.acquireForService(service)) {
		stdoutput.printf("acquireForService():\n");
		stdoutput.printf("%s\n",cred.getStatus());
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
		for (uint64_t i=0; i<cred.getDesiredMechanismCount(); i++) {
			gssmechanism	*mech=cred.getDesiredMechanism(i);
			if (mech) {
				stdoutput.printf("    %s\n",
						mech->getString());
			}
		}
		stdoutput.write("  actual mechanisms:\n");
		for (uint64_t i=0; i<cred.getActualMechanismCount(); i++) {
			gssmechanism	*mech=cred.getActualMechanism(i);
			if (mech) {
				stdoutput.printf("    %s\n",
						mech->getString());
			}
		}
		stdoutput.write("}\n");
	}

	// configure the security context
	gsscontext	ctx;
	ctx.setCredentials(&cred);

	// create an inet socket server
	inetsocketserver	iss;

	// attach the security context
	iss.setSecurityContext(&ctx);

	// listen
	if (iss.listen(NULL,port,0)) {

		for (;;) {

			// accept (will also accept context)
			filedescriptor	*fd=iss.accept();
			if (!fd) {
				if (error::getErrorNumber()) {
					stdoutput.printf(
						"accept failed (1): %s\n",
						error::getErrorString());
				} else {
					stdoutput.printf(
						"accept failed (2): %s\n",
						ctx.getMechanismMinorStatus());
				}
				continue;
			}
			fd->setWriteBufferSize(65536);
			fd->setReadBufferSize(65536);

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

			stdoutput.printf("clientSession {\n");

			// read messages from the client...
			for (;;) {

				uint64_t	msgsize;
				ssize_t	sizeread=fd->read(&msgsize);
				if (sizeread<=0) {
					if (sizeread==0) {
						stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  read() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					}
					break;
				} else if (sizeread!=sizeof(uint64_t)) {
					stdoutput.printf(
						"  read() size failed (3): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					break;
				}

				unsigned char	*msg=new unsigned char[msgsize];
				sizeread=fd->read(msg,msgsize);
				if (sizeread<=0) {
					if (sizeread==0) {
						stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  read() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					}
					delete[] msg;
					break;
				} else if (sizeread!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  read() size failed (3): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					delete[] msg;
					break;
				}

				stdoutput.printf("\n  Received message... "
						"(size=%d):\n  ",msgsize);
				stdoutput.safePrint(msg,
					(msgsize<=70)?msgsize:70);
				if (msgsize>70) {
					stdoutput.write("...");
				}
				stdoutput.write('\n');
				stdoutput.printf("\n  Sending response...");

				ssize_t	sizewritten=fd->write(msgsize);
				if (sizewritten<=0) {
					if (sizewritten==0) {
						stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  write() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					}
					delete[] msg;
					break;
				} else if (sizewritten!=sizeof(uint64_t)) {
					stdoutput.printf(
						"  write() size failed (3): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					delete[] msg;
					break;
				}

				sizewritten=fd->write(msg,msgsize);
				if (sizewritten<=0) {
					if (sizewritten==0) {
						stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  write() size failed (2): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					}
					delete[] msg;
					break;
				} else if (sizewritten!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  write() size failed (3): "
						"%s\n",
						ctx.getMechanismMinorStatus());
					delete[] msg;
					break;
				}

				delete[] msg;

				if (!fd->flushWriteBuffer(-1,-1)) {
					stdoutput.printf(
						"\n  flushWriteBuffer() msg "
						"failed\n");
					break;
				}

				stdoutput.printf(" success\n");
			}

			stdoutput.printf("}\n");

			// close and delete the client socket
			delete fd;
		}
	}

	stdoutput.printf("error listening on port %d\n%s\n",
				port,error::getErrorString());
	process::exit(1);
}
