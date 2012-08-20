// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/chat.h>
#include <rudiments/xmldom.h>
#include <rudiments/xmldomnode.h>
#include <rudiments/regularexpression.h>
#include <rudiments/snooze.h>
#include <rudiments/character.h>

//#define DEBUG_CHAT 1

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
	printf("runScript(readfd=%d,writefd=%d,i\n\"%s\")\n",
				pvt->_readfd->getFileDescriptor(),
				pvt->_writefd->getFileDescriptor(),
				script);
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

		char	*unescaped=NULL;
		int32_t	result=RESULT_SUCCESS;
		if (!charstring::compare(node->getName(),"timeout")) {
			pvt->_timeout=charstring::toInteger(
				node->getAttributeValue("seconds"));
		} else if (!charstring::compare(node->getName(),"abort")) {
			charUnescape(node->getAttributeValue("string"),
							&unescaped,false);
			appendAbortString(unescaped);
		} else if (!charstring::compare(node->getName(),
							"clearabort")) {
			clearAbortStrings();
		} else if (!charstring::compare(node->getName(),"send")) {
			charUnescape(node->getAttributeValue("string"),
							&unescaped,false);
			result=send(unescaped,variables);
		} else if (!charstring::compare(node->getName(),"expect")) {
			charUnescape(node->getAttributeValue("string"),
							&unescaped,false);
			result=expect(unescaped,abort);
		} else if (!charstring::compare(node->getName(),"flush")) {
			flush();
		}
		delete[] unescaped;
		unescaped=NULL;

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
	pvt->_aborts.append(charstring::duplicate(string));
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
	printf("expecting \"");
	charstring::safePrint(string);
	printf(" (%ld second timeout)...\n",pvt->_timeout);
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
				if (ch=='N') {
					// null
					ch='\0';
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
			}

			#ifdef DEBUG_CHAT
			printf("%c",ch);
			fflush(stdout);
			#endif

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

void chat::charUnescape(const char *str, char **newstr, bool second) {

	if (!charstring::length(str)) {
		*newstr=NULL;
		return;
	}

	*newstr = new char[charstring::length(str)+1];

	unsigned long i=0;
	for (char ch; (ch=*str); str++) {
		if (ch=='\\') {
			ch=*(++str);
			if (!ch) {
				break;
			} else if (ch=='b') {
				// backspace
				ch='\b';
			} else if (ch>='0' && ch<='8') {
				// octal digits
				char	val=(ch-'0')*64;
				ch=*(++str);
				if (ch>='0' && ch<='8') {
					val=val+(ch-'0')*8;
					ch=*(++str);
				}
				if (ch>='0' && ch<='8') {
					val=val+(ch-'0');
					ch=val;
				}
			} else if (ch=='r') {
				ch='\r';
			} else if (ch=='n') {
				ch='\n';
			} else if (ch=='\\'){
				if (second) {
					(*newstr)[i++]=ch;
				}
			} else {
				// put the \ back in the new str because it
				// isn't a standard escape character.
				(*newstr)[i++]='\\';
			}
		}
		(*newstr)[i++]=ch;
	}
	(*newstr)[i]='\0';
}


#ifdef RUDIMENTS_NAMESPACE
}
#endif
