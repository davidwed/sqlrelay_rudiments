// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/inetserversocket.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int passwd_cb(char *buf, int size, int rwflag, void *userdata) {
	strncpy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return strlen(buf);
}

class myserver : public daemonprocess, public inetserversocket {
	public:
			myserver() : daemonprocess(), inetserversocket() {}
		void	listen();
};


void	myserver::listen() {


	// make sure that only one instance is running
	int	pid=checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	createPidFile("/tmp/svr.pidfile",permissions::ownerReadWrite());

	SSL_library_init();
	SSL_CTX	*ctx=SSL_CTX_new(SSLv2_server_method());
	SSL_CTX_set_mode(ctx,SSL_MODE_ENABLE_PARTIAL_WRITE);
	SSL_CTX_use_certificate_chain_file(ctx,"server.pem");
	SSL_CTX_set_default_passwd_cb(ctx,passwd_cb);
	SSL_CTX_set_default_passwd_cb_userdata(ctx,(void *)"password");
	SSL_CTX_use_PrivateKey_file(ctx,"server.pem",SSL_FILETYPE_PEM);
	SSL_CTX_load_verify_locations(ctx,"root.pem",0);
	#if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
		SSL_CTX_set_verify_depth(ctx,1);
	#endif

	BIO	*bio=BIO_new_file("dh1024.pem","r");
	#if (OPENSSL_VERSION_NUMBER < 0x00904000)
		DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL);
	#else
		DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
	#endif
	BIO_free(bio);
	SSL_CTX_set_tmp_dh(ctx,dh);

	// listen on inet socket port 8000
	if (!listenOnSocket(NULL,8000,15)) {
		printf("couldn't listen on port 8000\n");
		exit(0);
	}

	// loop...
	for (;;) {

		setSSLContext(ctx);

		// accept a client connection
		datatransport	*clientsock=acceptClientConnection();

		if (clientsock) {

			// read 5 bytes from the client and display it
			char	buffer[6];
			buffer[5]=(char)NULL;
			clientsock->read((char *)buffer,5);
			printf("%s\n",buffer);

			// write "hello" back to the client
			clientsock->write("hello",5);

		} else {
			printf("accept failed\n");
		}

		// close the socket and clean up
		clientsock->close();
		delete clientsock;
	}
}


myserver	*mysvr;

// define a function to shut down the process cleanly
RETSIGTYPE	shutDown() {
	printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	unlink("/tmp/svr.pidfile");
	exit(0);
}


int main(int argc, const char **argv) {

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	mysvr->handleShutDown((RETSIGTYPE *)shutDown);
	mysvr->handleCrash((RETSIGTYPE *)shutDown);

	mysvr->listen();
}
