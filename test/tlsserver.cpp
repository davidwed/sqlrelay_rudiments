// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/tls.h>
#include <rudiments/commandline.h>
#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);

	if (cmdl.found("help")) {
		stdoutput.printf("tlsserver [-port port] [-version version] [-cert cert] [-ciphers ciphers] [-validate (yes|no)] [-depth depth] [-ca ca] [-commonname name]\n");
		process::exit(0);
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
	bool	validate=false;
	if (cmdl.found("validate")) {
		validate=!charstring::compare(cmdl.getValue("validate"),"yes");
	}
	uint16_t	depth=9;
	if (cmdl.found("depth")) {
		depth=charstring::toUnsignedInteger(cmdl.getValue("depth"));
	}
	const char	*ca=NULL;
	if (cmdl.found("ca")) {
		ca=cmdl.getValue("ca");
	}
	const char	*commonname="client.localdomain";
	if (cmdl.found("commonname")) {
		commonname=cmdl.getValue("commonname");
	}

	// configure the security context
	tlscontext	ctx;
	ctx.setProtocolVersion(version);
	ctx.setCertificateChainFile(cert);
	ctx.setPrivateKeyPassword("password");
	ctx.setCiphers(ciphers);
	ctx.setValidatePeer(validate);
	ctx.setValidationDepth(depth);
	ctx.setCertificateAuthority(ca);

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
						ctx.getErrorString());
				}
				continue;
			}
			fd->setWriteBufferSize(65536);
			fd->setReadBufferSize(65536);

			if (validate) {

				// make sure the client sent a certificate
				tlscertificate	*pcert=ctx.getPeerCertificate();
				if (!pcert) {
					stdoutput.printf("peer sent no "
							"certificate\n%s\n",
							ctx.getErrorString());
					fd->close();
					delete fd;
					continue;
				}

				// Make sure the commonname in the certificate
				// is the one we expect it to be
				const char	*cn=pcert->getCommonName();
				if (charstring::compareIgnoringCase(
							cn,commonname)) {
					stdoutput.printf("%s!=%s\n",
							cn,commonname);
					fd->close();
					delete fd;
					continue;
				}

				stdoutput.printf("client certificate {\n");
				stdoutput.printf("  version: %d\n",
						pcert->getVersion());
				stdoutput.printf("  serial number: %lld\n",
						pcert->getSerialNumber());
				stdoutput.printf("  signature algorithm: %s\n",
						pcert->getSignatureAlgorithm());
				stdoutput.printf("  issuer: %s\n",
						pcert->getIssuer());
				stdoutput.printf("  valid-from: %s\n",
						pcert->getValidFrom()->
							getString());
				stdoutput.printf("  valid-to: %s\n",
						pcert->getValidTo()->
							getString());
				stdoutput.printf("  subject: %s\n",
						pcert->getSubject());
				stdoutput.printf(
					"  public key algorithm: %s\n",
					pcert->getPublicKeyAlgorithm());
				stdoutput.printf("  public key: ");
				stdoutput.safePrint(
					pcert->getPublicKey(),
					(pcert->getPublicKeyByteLength()<5)?
					pcert->getPublicKeyByteLength():5);
				stdoutput.printf("...\n");
				stdoutput.printf(
					"  public key length: %lld\n",
					pcert->getPublicKeyByteLength());
				stdoutput.printf("  public key bits: %lld\n",
					pcert->getPublicKeyBitLength());
				stdoutput.printf("  common name: %s\n",
					pcert->getCommonName());
				for (linkedlistnode< char * > *node=
					pcert->getSubjectAlternateNames()->
					getFirst();
					node; node=node->getNext()) {
					stdoutput.printf("    %s\n",
							node->getValue());
				}
				stdoutput.printf("}\n");
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
						"%s\n",ctx.getErrorString());
					}
					break;
				} else if (sizeread!=sizeof(uint64_t)) {
					stdoutput.printf(
						"  read() size failed (3): "
						"%s\n",ctx.getErrorString());
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
						"%s\n",ctx.getErrorString());
					}
					delete[] msg;
					break;
				} else if (sizeread!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  read() size failed (3): "
						"%s\n",ctx.getErrorString());
					delete[] msg;
					break;
				}

				stdoutput.printf("\nReceived message... "
						"(size=%d):\n",msgsize);
				stdoutput.safePrint(msg,
					(msgsize<=300)?msgsize:300);
				if (msgsize>300) {
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
						"%s\n",ctx.getErrorString());
					}
					delete[] msg;
					break;
				} else if (sizewritten!=sizeof(uint64_t)) {
					stdoutput.printf(
						"  write() size failed (3): "
						"%s\n",ctx.getErrorString());
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
						"%s\n",ctx.getErrorString());
					}
					delete[] msg;
					break;
				} else if (sizewritten!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  write() size failed (3): "
						"%s\n",ctx.getErrorString());
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

				stdoutput.printf("  success\n");
			}
	
			stdoutput.printf("}\n");

			// close and delete the client socket
			delete fd;

			// FIXME: necessary?
			ctx.close();
		}
	}

	stdoutput.printf("error listening on port %d\n%s\n",
				port,error::getErrorString());
	process::exit(1);
}
