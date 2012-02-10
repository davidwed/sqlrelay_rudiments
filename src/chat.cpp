// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/chat.h>
#include <rudiments/xmldom.h>
#include <rudiments/xmldomnode.h>
#include <rudiments/regularexpression.h>
#include <rudiments/snooze.h>
#include <rudiments/character.h>
#include <rudiments/serialport.h>

#define DEBUG_CHAT 1

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class chatprivate {
	friend class chat;
	private:
		const filedescriptor	*_readfd;
		const filedescriptor	*_writefd;

		long		_timeout;
		stringlist	_aborts;
};

chat::chat(const filedescriptor *fd) {
	pvt=new chatprivate;
	pvt->_readfd=fd;
	pvt->_writefd=fd;
	pvt->_timeout=45;
}

chat::chat(const filedescriptor *rfd, const filedescriptor *wfd) {
	pvt=new chatprivate;
	pvt->_readfd=rfd;
	pvt->_writefd=wfd;
	pvt->_timeout=45;
}

chat::~chat() {
	delete pvt;
}

int32_t chat::runScript(const char *script, char **abort) {
	return runScript(script,abort,NULL);
}

int32_t chat::runScript(const char *script, char **abort,
				constnamevaluepairs *variables) {

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

		int32_t	result=RESULT_SUCCESS;
		if (!charstring::compare(node->getName(),"timeout")) {
			pvt->_timeout=charstring::toInteger(
				node->getAttributeValue("seconds"));
		} else if (!charstring::compare(node->getName(),"abort")) {
			appendAbortString(node->getAttributeValue("string"));
		} else if (!charstring::compare(node->getName(),
							"clearabort")) {
			clearAbortStrings();
		} else if (!charstring::compare(node->getName(),"send")) {
			result=send(node->getAttributeValue("string"),
								variables);
		} else if (!charstring::compare(node->getName(),"expect")) {
			result=expect(node->getAttributeValue("string"),abort);
		} else if (!charstring::compare(node->getName(),"flush")) {
			flush();
		}

		if (result!=RESULT_SUCCESS) {
			#ifdef DEBUG_CHAT
			if (result==RESULT_ERROR) {
				printf("error\n");
			} else if (result==RESULT_TIMEOUT) {
				printf("timeout\n");
			} else if (result==RESULT_ABORT) {
				printf("abort\n");
			} else {
				printf("abort condition met\n");
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

void chat::appendAbortString(const char *string) {

	char	*newstring=new char[charstring::length(string)+1];

	// replace \\r and \\n
	int32_t	index=0;
	for (const char *ptr=string; *ptr; ptr++) {
		if (*ptr=='\\') {
			ptr++;
			if (*ptr=='n') {
				newstring[index]='\n';
			} else if (*ptr=='r') {
				newstring[index]='\r';
			} else if (*ptr=='\\') {
				newstring[index]='\\';
			} else if (!*ptr) {
				break;
			}
		} else {
			newstring[index]=*ptr;
		}
		index++;
	}
	newstring[index]='\0';

	pvt->_aborts.append(newstring);
}

void chat::clearAbortStrings() {
	for (stringlistnode *sln=pvt->_aborts.getFirstNode();
					sln; sln=sln->getNext()) {
		char	*abortstring=sln->getData();
		delete[] abortstring;
	}
	pvt->_aborts.clear();
}

void chat::flush() {
	#ifdef DEBUG_CHAT
	printf("flushing...\n");
	#endif
	bool	wasusingnonblocking=pvt->_readfd->isUsingNonBlockingMode();
	if (wasusingnonblocking) {
		pvt->_readfd->useNonBlockingMode();
	}
	// some devices don't support non-blocking mode,
	// bail if we have encountered one of them
	if (!pvt->_readfd->isUsingNonBlockingMode()) {
		#ifdef DEBUG_CHAT
		printf("device doesn't support non-blocking mode, bailing\n");
		#endif
		return;
	}
	char	buffer[80];
	for (;;) {
		ssize_t	bytesread=pvt->_readfd->read(buffer,sizeof(buffer));
		#ifdef DEBUG_CHAT
		charstring::safePrint(buffer,bytesread);
		printf("\n");
		#endif
		if (bytesread!=sizeof(buffer)) {
			break;
		}
	}
	if (!wasusingnonblocking) {
		pvt->_readfd->useBlockingMode();
	}
}

int32_t chat::expect(const char *string, char **abort) {

	if (abort) {
		*abort=NULL;
	}

	#ifdef DEBUG_CHAT
	printf("expecting \"%s\" (%ld second timeout)...\n",
						string,pvt->_timeout);
	#endif

	stringbuffer	response;
	char		ch;

	// loop until we get the expected response,
	// an abort string, abort, error or timeout
	for (;;) {

		// read a character
		int32_t	result=pvt->_readfd->read(&ch,pvt->_timeout,0);

		#ifdef DEBUG_CHAT
		character::safePrint(ch);
		fflush(stdout);
		#endif

		// if we got an error, timeout or abort, return
		if (result!=sizeof(char)) {
			#ifdef DEBUG_CHAT
			printf("\n");
			printf("failed: ");
			if (result==RESULT_TIMEOUT) {
				printf("timed out...\n");
			} else if (result==RESULT_ERROR) {
				printf("error...\n");
			} else {
				// some compilers complain without this cast
				printf("result was: %d\n",(int)result);
			}
			#endif
			return result;
		}

		// append the character to the response
		// (unless it's a NULL, we shouldn't get these, but when the
		// modem is first initialized, there may be a few in it's
		// send buffer from whatever it was doing before and we may
		// accidentally get some)
		if (ch!='\0') {
			response.append(ch);
		}

		// compare to abort strings, if the result matches, then
		// return the (two-based) index of the abort string
		int32_t	index=2;
		for (stringlistnode *sln=pvt->_aborts.getFirstNode();
						sln; sln=sln->getNext()) {

			char	*abortstring=sln->getData();
			if (regularexpression::match(response.getString(),
								abortstring)) {
				#ifdef DEBUG_CHAT
				printf("\nabort: \"");
				charstring::safePrint(response.getString());
				printf("\" matched \"");
				charstring::safePrint(abortstring);
				printf("\"\n");
				#endif
				if (abort) {
					*abort=charstring::
						duplicate(abortstring);
				}
				return index;
			}
			index++;
		}

		// look for the expected string
		if (regularexpression::match(response.getString(),string)) {
			#ifdef DEBUG_CHAT
			printf("\nsuccess: \"");
			charstring::safePrint(response.getString());
			printf("\" matched \"");
			charstring::safePrint(string);
			printf("\"\n");
			#endif
			return RESULT_SUCCESS;
		}
	}
}

int32_t chat::send(const char *string, constnamevaluepairs *variables) {

	#ifdef DEBUG_CHAT
	printf("sending:\n");
	#endif

	// write the string, character at a time, processing special characters
	int32_t	result=RESULT_SUCCESS;
	if (string) {

		// set up an instance of the serial port class, just in case
		// we need to send a break sequence
		serialport	sp;
		sp.setFileDescriptor(pvt->_writefd->getFileDescriptor());

		for (const char *ptr=string; *ptr; ptr++) {

			if (variables) {
				result=substituteVariables(&ptr,variables);
				if (result!=RESULT_SUCCESS) {
					break;
				}
			}

			char	ch=*ptr;
			if (ch=='\\') {
				ptr++;
				ch=*ptr;
				if (!ch) {
					#ifdef DEBUG_CHAT
					printf("\n");
					#endif
					break;
				} else if (ch=='b') {
					// backspace
					ch='\b';
					#ifdef DEBUG_CHAT
					printf("\\b");
					fflush(stdout);
					#endif
				} else if (ch=='K') {
					// break
					#ifdef DEBUG_CHAT
					printf("\\K");
					fflush(stdout);
					#endif
					if (!sp.sendBreak(0)) {
						#ifdef DEBUG_CHAT
						printf("\n");
						#endif
						break;
					}
					continue;
				} else if (ch>='0' && ch<='8') {
					// octal digits
					#ifdef DEBUG_CHAT
					printf("\\%c",ch);
					fflush(stdout);
					#endif
					char	val=(ch-'0')*64;
					ptr++;
					if (ch>='0' && ch<='8') {
						#ifdef DEBUG_CHAT
						printf("%c",ch);
						fflush(stdout);
						#endif
						val=val+(ch-'0')*8;
						ptr++;
					}
					if (ch>='0' && ch<='8') {
						#ifdef DEBUG_CHAT
						printf("%c",ch);
						fflush(stdout);
						#endif
						val=val+(ch-'0');
						ch=val;
					}
				} else if (ch=='N') {
					// null
					#ifdef DEBUG_CHAT
					printf("\\0");
					fflush(stdout);
					#endif
					ch='\0';
				} else if (ch=='r') {
					#ifdef DEBUG_CHAT
					printf("\\r");
					fflush(stdout);
					#endif
					ch='\r';
				} else if (ch=='n') {
					#ifdef DEBUG_CHAT
					printf("\\n");
					fflush(stdout);
					#endif
					ch='\n';
				} else if (ch=='p') {
					#ifdef DEBUG_CHAT
					printf("\ndecisleep\n");
					#endif
					snooze::microsnooze(0,100000);
					continue;
				} else if (ch=='d') {
					#ifdef DEBUG_CHAT
					printf("\nsleep\n");
					#endif
					snooze::macrosnooze(1);
					continue;
				}
			} else if (*ptr=='^') {
				// control characters
				ptr++;
				if (!*ptr) {
					#ifdef DEBUG_CHAT
					printf("\n");
					#endif
				}
				ch=*ptr-'A'+1;
				if (ch<0) {
					ch=0;
				}
				#ifdef DEBUG_CHAT
				printf("^%c",*ptr);
				fflush(stdout);
				#endif
			#ifdef DEBUG_CHAT
			} else {
				printf("%c",ch);
				fflush(stdout);
			#endif
			}

			result=pvt->_writefd->write(ch);
			if (result!=sizeof(ch)) {
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


int32_t chat::substituteVariables(const char **ch,
					constnamevaluepairs *variables) {
			
	// look for $(variable), make sure we don't just have $(\0
	const char	*str=*ch;
	if (charstring::compare(str,"$(") && *(str+2)) {

		for (constnamevaluepairslistnode *nln=
			(constnamevaluepairslistnode *)
			variables->getList()->getFirstNode(); nln;
			nln=(constnamevaluepairslistnode *)nln->getNext()) {

			const char	*variable=nln->getData()->getKey();
			ssize_t	varlen=charstring::length(variable);
			const char	*value=nln->getData()->getData();
			if (!charstring::compare(variable,str+2,varlen) &&
							*(str+2+varlen)==')') {
				ssize_t	result=pvt->_writefd->write(value);
				#ifdef DEBUG_CHAT
				printf("%s",value);
				#endif
				if (result!=
					(ssize_t)charstring::length(value)) {
					#ifdef DEBUG_CHAT
					printf("\n");
					#endif
					return result;
				}
				*ch=*ch+3+varlen;
			}
		}
	}
	return RESULT_SUCCESS;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
