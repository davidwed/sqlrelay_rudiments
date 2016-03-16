// Copyright (c) 2015-2016  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

static void usage() {
	stdoutput.printf("tlsclient "
			"[-host host] [-port port] "
			"[-version version] [-cert cert] [-ciphers ciphers] "
			"[-validate (yes|no)] [-depth depth] [-ca ca] "
			"[-commonname name] "
			"[-ccount count] [-mcount count] [-dcount count]\n");
}

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);
	if (cmdl.found("help")) {
		usage();
		process::exit(0);
	}
	const char	*host="127.0.0.1";
	if (cmdl.found("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=9000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}
	const char	*version=NULL;
	if (cmdl.found("version")) {
		version=cmdl.getValue("version");
	}
	const char	*cert=NULL;
	if (cmdl.found("cert")) {
		cert=cmdl.getValue("cert");
	}
	const char	*ciphers=NULL;
	if (cmdl.found("ciphers")) {
		ciphers=cmdl.getValue("ciphers");
	}
	bool	validate=true;
	if (cmdl.found("validate")) {
		validate=charstring::compare(cmdl.getValue("validate"),"no");
	}
	uint16_t	depth=9;
	if (cmdl.found("depth")) {
		depth=charstring::toUnsignedInteger(cmdl.getValue("depth"));
	}
	const char	*ca=NULL;
	if (cmdl.found("ca")) {
		ca=cmdl.getValue("ca");
	}
	const char	*commonname="server.localdomain";
	if (cmdl.found("commonname")) {
		commonname=cmdl.getValue("commonname");
	}
	const char	*msg="hello";
	if (cmdl.found("message")) {
		msg=cmdl.getValue("message");
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

	// duplicate the message the
	// specified number of times...
	bytebuffer	msgbuf;
	for (int64_t i=0; i<dcount; i++) {
		msgbuf.append(msg)->append(' ');
	}

{
	// configure the security context
	tlscontext	ctx;
	ctx.setProtocolVersion(version);
	ctx.setCertificateChainFile(cert);
	ctx.setPrivateKeyPassword("password");
	ctx.setCiphers(ciphers);
	ctx.setValidatePeer(validate);
	ctx.setValidationDepth(depth);
	ctx.setCertificateAuthority(ca);

	// create an inet socket client
	inetsocketclient	fd;
	fd.setWriteBufferSize(65536);
	fd.setReadBufferSize(65536);

	// attach the security context
	fd.setSecurityContext(&ctx);

	// loop, having sessions with the server
	for (int64_t i=0; i<ccount; i++) {

		// connect 
		if (fd.connect(host,port,-1,-1,1,1)!=RESULT_SUCCESS) {
			if (error::getErrorNumber()) {
				stdoutput.printf("connect failed (1): %s\n",
							error::getErrorString());
			} else {
				stdoutput.printf("connect failed (2): %s\n",
							ctx.getErrorString());
			}
			continue;
		}

		// make sure the server sent a certificate
		if (validate) {
			tlscertificate	*pcert=ctx.getPeerCertificate();
			if (!pcert) {
				stdoutput.printf(
					"peer sent no certificate\n%s\n",
					ctx.getErrorString());
				fd.close();
				delete pcert;
				continue;
			}

			// Make sure the commonname in the certificate
			// is the one we expect it to be.
			const char	*cn=pcert->getCommonName();
			if (charstring::compareIgnoringCase(cn,commonname)) {
				stdoutput.printf("%s!=%s\n",cn,commonname);
				fd.close();
				delete pcert;
				continue;
			}

			stdoutput.printf("server certificate {\n");
			stdoutput.printf("  version: %d\n",
					pcert->getVersion());
			stdoutput.printf("  serial number: %lld\n",
					pcert->getSerialNumber());
			stdoutput.printf("  signature algorithm: %s\n",
					pcert->getSignatureAlgorithm());
			stdoutput.printf("  issuer: %s\n",
					pcert->getIssuer());
			stdoutput.printf("  valid-from: %s\n",
					pcert->getValidFrom()->getString());
			stdoutput.printf("  valid-to: %s\n",
					pcert->getValidTo()->getString());
			stdoutput.printf("  subject: %s\n",
					pcert->getSubject());
			stdoutput.printf("  public key algorithm: %s\n",
					pcert->getPublicKeyAlgorithm());
			stdoutput.printf("  public key: ");
			stdoutput.safePrint(pcert->getPublicKey(),
					(pcert->getPublicKeyByteLength()<5)?
					pcert->getPublicKeyByteLength():5);
			stdoutput.printf("...\n");
			stdoutput.printf("  public key length: %lld\n",
					pcert->getPublicKeyByteLength());
			stdoutput.printf("  public key bits: %lld\n",
					pcert->getPublicKeyBitLength());
			stdoutput.printf("  common name: %s\n",
					pcert->getCommonName());
			stdoutput.printf("  subject alternate names:\n");
			for (linkedlistnode< char * > *node=
				pcert->getSubjectAlternateNames()->getFirst();
				node; node=node->getNext()) {
				stdoutput.printf("    %s\n",node->getValue());
			}
			stdoutput.printf("}\n");

			delete pcert;
		}

		stdoutput.printf("serverSession {\n");

		// write the message to the server,
		// the specified number of times
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
						ctx.getErrorString());
					break;
				}
			} else if (sizewritten!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  write() size failed (3): %s\n",
					ctx.getErrorString());
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
						ctx.getErrorString());
					break;
				}
			} else if (sizewritten!=(ssize_t)msgbuf.getSize()) {
				stdoutput.printf(
					"  write() msg failed (3): %s\n",
					ctx.getErrorString());
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
				(msgbuf.getSize()<=80)?msgbuf.getSize():80);
			if (msgbuf.getSize()>80) {
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
						ctx.getErrorString());
					break;
				}
			} else if (sizeread!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  read() size failed (3): %s\n",
					ctx.getErrorString());
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
						ctx.getErrorString());
					delete[] msg;
					break;
				}
			} else if (sizeread!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  read() msg failed (3): %s\n",
					ctx.getErrorString());
				delete[] msg;
				break;
			}

			stdoutput.printf("  success\n");

			delete[] msg;
		}

		stdoutput.printf("}\n");

		// close the connection to the server
		fd.close();
	}
}

	process::exit(0);
}
