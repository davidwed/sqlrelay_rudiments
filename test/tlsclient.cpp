// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/tls.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	commandline	cmdl(argc,argv);
	if (cmdl.found("help")) {
		stdoutput.printf("tlsclient [-host host] [-port port] [-cert cert] [-ciphers ciphers] [-validate (yes|no)] [-depth depth] [-ca ca] [-commonname name]\n");
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

	// create the tls context
	tlscontext	ctx;
	ctx.setCertificateChainFile(cert);
	ctx.setPrivateKeyPassword("password");
	ctx.setCiphers(ciphers);
	ctx.setValidatePeer(validate);
	ctx.setValidationDepth(depth);
	ctx.setCertificateAuthority(ca);

	// create an inet socket client
	inetsocketclient	clnt;
	clnt.setSecurityContext(&ctx);

	// connect to a server on localhost, listening on port 9000
	if (clnt.connect(host,port,-1,-1,1,1)<0) {
		if (error::getErrorNumber()) {
			stdoutput.printf("connect failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("connect failed (2): %s\n",
						ctx.getErrorString());
		}
		clnt.close();
		process::exit(1);
	}

	// make sure the server sent a certificate
	if (validate) {
		tlscertificate	*certificate=ctx.getPeerCertificate();
		if (!certificate) {
			stdoutput.printf("peer sent no certificate\n%s\n",
							ctx.getErrorString());
			clnt.close();
			process::exit(1);
		}

		// Make sure the commonname in the certificate is the one we
		// expect it to be.
		const char	*cn=certificate->getCommonName();
		if (charstring::compareIgnoringCase(cn,commonname)) {
			stdoutput.printf("%s!=%s\n",cn,commonname);
			clnt.close();
			process::exit(1);
		}

		stdoutput.printf("server certificate {\n");
		stdoutput.printf("  common name: %s\n",cn);
		stdoutput.printf("}\n\n");
	}

	// write "hello" to the server
	ssize_t	sizewritten=clnt.write("hello",5);
	if (sizewritten<=0) {
		if (sizewritten==0) {
			stdoutput.printf("write failed (0): eof\n");
		} else if (error::getErrorNumber()) {
			stdoutput.printf("write failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("write failed (2): %s\n",
						ctx.getErrorString());
		}
	}

	// read 10 bytes from the server and display them
	char	buffer[11];
	ssize_t	sizeread=clnt.read(buffer,10);
	if (sizeread<=0) {
		if (sizeread==0) {
			stdoutput.printf("read failed (0): eof\n");
		} else if (error::getErrorNumber()) {
			stdoutput.printf("read failed (1): %s\n",
						error::getErrorString());
		} else {
			stdoutput.printf("read failed (2): %s\n",
						ctx.getErrorString());
		}
	} else {
		buffer[sizeread]=(char)NULL;
		stdoutput.printf("%s\n",buffer);
	}

	// close the connection to the server
	clnt.close();
}
