#include <rudiments/chat.h>
#include <rudiments/xmldom.h>
#include <rudiments/xmldomnode.h>
#include <rudiments/regularexpression.h>
#include <rudiments/intervaltimer.h>

#define DEBUG_CHAT 1

chat::chat(filedescriptor *readfd, filedescriptor *writefd) {
	this->readfd=readfd;
	this->writefd=writefd;
	timeout=45;
}

chat::~chat() {}

int chat::runScript(const char *script) {

	#ifdef DEBUG_CHAT
	printf("runScript(\"%s\")\n",script);
	#endif

	// if there was no script, just return success
	if (!script || !script[0]) {
		return RESULT_SUCCESS;
	}

	// parse the script
	xmldom	xmld;
	if (!xmld.parseString(script)) {
		return RESULT_ERROR;
	}

	// get the <script> tag
	xmldomnode	*root=xmld.getRootNode();
	xmldomnode	*scriptnode=root->getFirstTagChild("script");
	if (root->isNullNode() || scriptnode->isNullNode()) {
		return RESULT_ERROR;
	}
	root->cascadeOnDelete();

	// run through the children of the <script> tag...
	for (xmldomnode *node=scriptnode->getFirstTagChild();
			!node->isNullNode();
			node=node->getNextTagSibling()) {

		int	result=RESULT_SUCCESS;
		if (!charstring::compare(node->getName(),"timeout")) {
			timeout=charstring::toLong(
				node->getAttributeValue("seconds"));
		} else if (!charstring::compare(node->getName(),"abort")) {
			aborts.append(node->getAttributeValue("string"));
		} else if (!charstring::compare(node->getName(),"clearabort")) {
			aborts.clear();
		} else if (!charstring::compare(node->getName(),"send")) {
			result=send(node->getAttributeValue("string"));
		} else if (!charstring::compare(node->getName(),"expect")) {
			result=expect(node->getAttributeValue("string"));
		}

		if (result!=RESULT_SUCCESS) {
			#ifdef DEBUG_CHAT
			if (result==RESULT_ERROR) {
				printf("error\n");
			} else if (result==RESULT_TIMEOUT) {
				printf("timeout\n");
			} else if (result==RESULT_ABORT) {
				printf("abort\n");
			}
			#endif
			return result;
		}
	}

	#ifdef DEBUG_CHAT
	printf("success!\n");
	#endif
	return RESULT_SUCCESS;
}

int chat::expect(const char *string) {

	#ifdef DEBUG_CHAT
	printf("expecting \"%s\"...\n",string);
	#endif

	stringbuffer	response;
	char		ch;

	// loop until we get the expected response,
	// an abort string, abort, error or timeout
	for (;;) {

		// read a character
		int	result=readfd->read(&ch,timeout,0);
		#ifdef DEBUG_CHAT
		if (ch=='\r') {
			printf("\\r");
		} else if (ch=='\n') {
			printf("\\n");
		} else {
			printf("%c",ch);
		}
		fflush(stdout);
		#endif

		// if we got an error, timeout or abort, return
		if (result!=sizeof(char)) {
			#ifdef DEBUG_CHAT
			printf("\n");
			#endif
			return result;
		}

		// append the character to the response
		response.append(ch);

		// compare to abort strings, if the result matches, then
		// return the (two-based) index of the abort string
		int	index=2;
		for (stringlistnode *sln=aborts.getNodeByIndex(0);
						sln; sln=sln->getNext()) {

			char	*abortstring=sln->getData();
			if (regularexpression::match(response.getString(),
								abortstring)) {
				#ifdef DEBUG_CHAT
				printf("\nabort: \"%s\" matched \"%s\"\n",
					response.getString(),abortstring);
				#endif
				return index;
			}
			index++;
		}

		// look for the expected string
		if (regularexpression::match(response.getString(),string)) {
			#ifdef DEBUG_CHAT
			printf("\nsuccess: \"%s\" matched \"%s\"\n",
					response.getString(),string);
			#endif
			return RESULT_SUCCESS;
		}
	}
}

int chat::send(const char *string) {

	#ifdef DEBUG_CHAT
	printf("sending:\n");
	#endif

	// write the string, character at a time, processing special characters
	int	result=RESULT_SUCCESS;
	if (string) {
		for (int index=0; string[index]; index++) {
			
			// process special characters
			char	ch=string[index];
			if (ch=='\\') {
				index++;
				ch=string[index];
				if (!ch) {
					break;
				} else if (ch=='r') {
					ch='\r';
				} else if (ch=='n') {
					ch='\n';
				} else if (ch=='p') {
					#ifdef DEBUG_CHAT
					printf("\ndecisleep\n");
					#endif
					intervaltimer::microsleep(0,100000);
				} else if (ch=='d') {
					#ifdef DEBUG_CHAT
					printf("\nsleep\n");
					#endif
					intervaltimer::sleep(1);
					continue;
				}
			}

			#ifdef DEBUG_CHAT
			if (ch=='\r') {
				printf("\\r");
			} else if (ch=='\n') {
				printf("\\n");
			} else {
				printf("%c",ch);
			}
			fflush(stdout);
			#endif

			result=writefd->write(ch);
			if (result!=RESULT_SUCCESS) {
				#ifdef DEBUG_CHAT
				printf("\n");
				#endif
				break;
			}
		}
	}

	#ifdef DEBUG_CHAT
	printf("\n");
	#endif
	return result;
}
