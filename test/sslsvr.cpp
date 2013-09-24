// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAS_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifdef RUDIMENTS_HAS_SSL
int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}

class myserver : public inetsocketserver {
	public:
			myserver() : inetsocketserver() {}
		void	listen();
};


void myserver::listen() {


	// make sure that only one instance is running
	int	pid=process::checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	process::createPidFile("/tmp/svr.pidfile",
				permissions::ownerReadWrite());

	// initalize the libarary
	SSL_library_init();
	SSL_load_error_strings();

	// Create a new server context usng SSLv2/3
	SSL_CTX	*ctx=SSL_CTX_new(SSLv23_server_method());

	// in cases where a re-negotiation must take place during an SSL_read
	// or SSL_write, automatically re-negotiate, then retry the read/write
	// instead of causing the read/write to fail
	SSL_CTX_set_mode(ctx,SSL_MODE_AUTO_RETRY);

	// load the server's certificate chain
	SSL_CTX_use_certificate_chain_file(ctx,"server.pem");

	// if the private key requires a password in order to read it,
	// supply "password"
	SSL_CTX_set_default_passwd_cb(ctx,passwdCallback);
	SSL_CTX_set_default_passwd_cb_userdata(ctx,(void *)"password");

	// load the server's private key
	SSL_CTX_use_PrivateKey_file(ctx,"server.pem",SSL_FILETYPE_PEM);

	// Instruct the server to request the client's certificate.  Servers
	// always send certificates to clients, but in order for a client to
	// send a certificate to a server, the server must request it.
	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);

	// load certificates for the signing authorities that we trust
	SSL_CTX_load_verify_locations(ctx,"ca.pem",0);
	#if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
		// client certificates must be directly signed
		// by one of the signing authorities that we trust
		SSL_CTX_set_verify_depth(ctx,1);
	#endif

	// Instruct the context to use an ephemeral
	// dh key for encrypting the session.
	SSL_CTX_set_options(ctx,SSL_OP_SINGLE_DH_USE);

	// Get parameters for generating the dh key from dh1024.pem.
	BIO	*bio=BIO_new_file("dh1024.pem","r");
	#if (OPENSSL_VERSION_NUMBER < 0x00904000)
		DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL);
	#else
		DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
	#endif
	BIO_free(bio);

	// Supply the context with the dh parameters for generating the key.
	SSL_CTX_set_tmp_dh(ctx,dh);

	// listen on inet socket port 8000
	if (!inetsocketserver::listen(NULL,8000,15)) {
		stdoutput.printf("couldn't listen on port 8000\n");
		process::exit(0);
	}

	// loop...
	for (;;) {

		// attach the ssl context to the server
		setSSLContext((void *)ctx);

		// accept a client connection
		filedescriptor	*clientsock=accept();

		if (clientsock) {

			SSL	*ssl=(SSL *)clientsock->getSSL();

			// make sure the client sent a certificate
			X509	*certificate=SSL_get_peer_certificate(ssl);
			if (!certificate) {
				stdoutput.printf("peer sent no certificate\n");
				clientsock->close();
				delete clientsock;
				continue;
			}

			// make sure the certificate was valid
			long	result=SSL_get_verify_result(ssl);
			if (result!=X509_V_OK) {
				stdoutput.printf("SSL_get_verify_result failed: %ld\n",
									result);
				clientsock->close();
				continue;
			}

			// Make sure the commonname in the certificate is the
			// one we expect it to be (client.localdomain).
			// (we may also want to check the subject name field or
			// certificate extension for the commonname because
			// sometimes it's there instead of in the commonname
			// field)
			char	commonname[256];
			X509_NAME_get_text_by_NID(
					X509_get_subject_name(certificate),
					NID_commonName,commonname,256);
			if (charstring::compareIgnoringCase(commonname,
							"client.localdomain")) {
				stdoutput.printf("%s!=client.localdomain\n",commonname);
				clientsock->close();
				delete clientsock;
				continue;
			}

			// read 5 bytes from the client and display it
			char	buffer[6];
			buffer[5]=(char)NULL;
			clientsock->read((char *)buffer,5);
			stdoutput.printf("%s\n",buffer);

			// write "hello" back to the client
			clientsock->write("hello",5);

		} else {

			// if errno>0 then we got a system error, otherwise we
			// got an SSL-specific error
			if (errno) {
				stdoutput.printf("accept failed: %s\n",
					error::getErrorString());
			} else {
				stdoutput.printf("accept failed: ");
				ERR_print_errors_fp(stdout);
				stdoutput.printf("\n");
			}
		}

		// close the socket and clean up
		clientsock->close();
		delete clientsock;
	}
}


myserver	*mysvr;

// define a function to shut down the process cleanly
void shutDown(int sig) {
	stdoutput.printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	file::remove("/tmp/svr.pidfile");
	process::exit(0);
}
#endif

int main(int argc, const char **argv) {

#ifdef RUDIMENTS_HAS_SSL
	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen();
#else
	stdoutput.printf("rudiments built without ssl support\n");
#endif
}
