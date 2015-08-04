#include <rudiments/hostentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	uint32_t	i;

	// get the host entry for IP address "127.0.0.1"
	hostentry	he;
	char	address[]={127,0,0,1};
	he.initialize(address,4,AF_INET);

	// print out details
	stdoutput.printf("	Name: %s\n",he.getName());
	stdoutput.printf("	Alias list:\n");
	for (i=0; he.getAliasList() && he.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",he.getAliasList()[i]);
	}
	stdoutput.printf("	Address type: %d\n",he.getAddressType());
	stdoutput.printf("	Address length: %d\n",he.getAddressLength());
	stdoutput.printf("	Address list:\n");
	for (i=0; he.getAddressList() && he.getAddressList()[i]; i++) {
		const char	*addr=he.getAddressList()[i];
		stdoutput.printf("		%d.%d.%d.%d\n",
					addr[0],addr[1],addr[2],addr[3]);
	}
	stdoutput.printf("\n");
}
