// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmlsax.h>
#include <rudiments/charstring.h>

#include <stdlib.h>

xmlsax::xmlsax() : errorhandler() {
	reset();
}

xmlsax::~xmlsax() {
	close();
}

void xmlsax::reset() {
	string=NULL;
	ptr=NULL;
	line=1;
}

bool xmlsax::tagStart(char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeName(char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeValue(char *value) {
	// by default, just return success
	return true;
}

bool xmlsax::text(char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::tagEnd(char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::comment(char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::cdata(char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::parseFile(const char *filename) {

	// reset string/line
	reset();

	// close any previously opened files, open the file, parse it, close
	// it again
	close();
#ifdef HAVE_MMAP
	// open the file
	bool retval;
	if ((retval=fl.open(filename,O_RDONLY))) {
		// Try to memorymap the file.  If it fails, that's ok, ptr will
		// be set to NULL from the previous call to reset() and will
		// cause getCharacter() to read from the file.
		if (mm.attach(fl.getFileDescriptor(),0,fl.getSize(),
						PROT_READ,MAP_PRIVATE)) {
			string=(char *)mm.getData();
			ptr=string;
		}
		retval=parse();
		if (ptr) {
			mm.detach();
		}
	}
#else
	bool	retval=(fl.open(filename,O_RDONLY) && parse());
#endif
	close();
	return retval;
}

bool xmlsax::parseString(const char *string) {

	// close any previously opened files
	close();

	// reset fd/line
	reset();

	// set string pointers
	ptr=this->string=(char *)string;

	return parse();
}

void xmlsax::close() {

	// reset string/fd/line
	reset();

	// close any previously opened files
	fl.close();
}

bool xmlsax::parse() {

	char	ch;

	// skip whitespace/check for an empty document
	if (!(ch=skipWhitespace((char)NULL))) {
		return true;
	}

	// parse the document body
	for (;;) {

		// parse the tag
		if (!parseTag(ch,&ch)) {
			return false;
		} else if (!ch) {
			break;
		}

		// parse text until we find another tag
		// Strictly speaking, if parseText returns 0 then there was
		// trailing text after the last tag.  There is so commonly
		// trailing text though, that we'll allow it.
		if (!parseText(ch,&ch)) {
			break;
		}
	}

	// document parsed successfully
	return true;
}

bool xmlsax::parseTag(char current, char *next) {

	char	ch=current;

	// make sure there's a <, skip any whitespace after it
	if (ch!='<' || !(ch=skipWhitespace(getCharacter()))) {
		parseTagFailed();
		return false;
	}

	// is this a standalone tag or end-tag?
	int	endtag=0;
	char	standalone=(char)NULL;
	if (ch=='!' || ch=='?') {
		standalone=ch;
	} else if (ch=='/') {
		endtag=1;
		if (!(ch=skipWhitespace(getCharacter()))) {
			parseTagFailed();
			return false;
		}
	}

	// get the tag name
	stringbuffer	name;
	if (!parseTagName(ch,&name,&ch)) {
		parseTagFailed();
		return false;
	}

	// handle comments and cdata
	if (!charstring::compare(name.getString(),"!--")) {
		if (!(ch=parseComment(ch))) {
			parseTagFailed();
			return false;
		}
		return (*next=getCharacter())!=(char)NULL;
	} else if (!charstring::compare(name.getString(),"![CDATA[")) {
		if (!(ch=parseCData(ch))) {
			parseTagFailed();
			return false;
		}
		return (*next=getCharacter())!=(char)NULL;
	}

	if (endtag) {

		// skip whitespace after the tag name and look for >
		if (!(ch=skipWhitespace(ch)) || ch!='>') {
			parseTagFailed();
			return false;
		}

	} else {

		// call the callback for tag start
		if (!tagStart(name.getString())) {
			return false;
		}

		// parse the attributes
		for (;;) {

			// skip any whitespace before the attribute
			if (!(ch=skipWhitespace(ch))) {
				parseTagFailed();
				return false;
			}
	
			if (ch=='/') {
				// empty tag
				endtag=1;
				if (!(ch=skipWhitespace(getCharacter())) ||
								ch!='>') {
					parseTagFailed();
					return false;
				}
				break;
			} else if (ch=='?') {
				// ? standalone tag, make sure there's a >
				// immediately following the ?
				if (!(ch=getCharacter()) || ch!='>') {
					parseTagFailed();
					return false;
				}
				break;
			} else if (ch=='>') {
				// normal tag
				break;
			} else {
				if (!(ch=parseAttribute(ch,standalone))) {
					parseTagFailed();
					return false;
				}
			}
		}
	}

	// if the tag was an empty or standalone tag,
	// call the callback for tag end
	if (endtag || standalone) {
		if (!tagEnd(name.getString())) {
			return false;
		}
	}

	// return the first character after the closing >
	*next=getCharacter();
	return true;
}

bool xmlsax::parseTagName(char current, stringbuffer *name, char *next) {

	int	bracketcount=0;

	// get characters and put them in the buffer
	char	ch=current;
	for (;;) {

		if (!ch) {

			// we should not run into a NULL or EOF here, if we
			// do then it's an error
			clearError();
			appendError("error: parseTagName() failed at line ");
			appendError(line);
			return false;

		} else if (ch=='[') {

			name->append(ch);

			// CDATA tags will have 2 brackets (![CDATA[)
			// if we've found 2 brackets, we're done
			bracketcount++;
			if (bracketcount==2) {
				// return the character after
				// the end of the name
				return (*next=getCharacter())!=(char)NULL;
			}

		} else if (ch==' ' || ch=='	' ||
				ch=='\n' || ch=='\r' || ch=='/' || ch=='>') {

			// if we find whitespace, a / or a > then we're done
			// parsing the name

			// return the character after the end of the name
			*next=ch;
			return true;
		} else {
			name->append(ch);
		}

		// look for comments
		if (name->getStringLength()==3 &&
			!charstring::compare(name->getString(),"!--")) {
			// return the character after the !--
			return (*next=getCharacter())!=(char)NULL;
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::parseComment(char current) {

	// create a buffer to store the comment
	stringbuffer	text;
	char		ch=current;

	for (;;) {

		// handle potential terminators
		if (ch=='-') {
			if (!(ch=getCharacter())) {
				return (char)NULL;
			} else if (ch=='-') {
				if (!(ch=getCharacter())) {
					return (char)NULL;
				} else if (ch=='>') {
					// call the comment callback
					comment(text.getString());
					return ch;
				} else {
					text.append("--");
				}
			} else {
				text.append('-');
			}
		}

		text.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			return (char)NULL;
		}
	}
}

char xmlsax::parseCData(char current) {

	// create a buffer to store the comment
	stringbuffer	text;
	char		ch=current;
	int		nest=0;

	for (;;) {

		// handle potential terminators
		if (ch=='[') {
			nest++;
		} else if (ch==']') {
			if (nest==0) {
				if (!(ch=getCharacter())) {
					return (char)NULL;
				} else if (ch==']') {
					// call the cdata callback
					cdata(text.getString());
					break;
				} else {
					text.append(']');
				}
			} else {
				nest--;
			}
		}

		text.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			return (char)NULL;
		}
	}

	// skip whitespace, get the next character and return it,
	// it should be a >
	if (!(ch=skipWhitespace(getCharacter())) || ch!='>') {
		return (char)NULL;
	}
	return ch;
}

char xmlsax::parseAttribute(char current, char standalone) {

	char		ch=current;
	stringbuffer	data;

	if (standalone!='!' ||
			(standalone=='!' &&
				ch!='"' && ch!='\'' && ch!='[' && ch!='(')) {

		// get the attribute name
		for (;;) {

			if (ch==' ' || ch=='	' || ch=='\n' || ch=='\r' ||
				(standalone && ch=='>')) {

				// if we got whitespace, skip past it
				if (!(ch=skipWhitespace(ch))) {
					parseAttributeFailed();
					return (char)NULL;
				}
	
				if (standalone) {
					// for standalone tags, return an
					// attribute value,
					if (!attributeValue(data.getString())) {
						return (char)NULL;
					}
					return ch;
				} else {
					// for non-standalone, make sure there's
					// an = after the whitespace
					if (ch!='=') {
						parseAttributeFailed();
						return (char)NULL;
					}
				}
	
			} else if (ch=='=') {
	
				// if we got an = then we've gotten the entire
				// name, terminate it and break out of the loop
				break;

			} else {
	
				// otherwise add the character
				// to the attribute name
				data.append(ch);
				if (!(ch=getCharacter())) {
					return (char)NULL;
				}
			}
		}

		// call the attribute name callback
		if (!attributeName(data.getString())) {
			return (char)NULL;
		}

		// skip any whitespace after the =, then look for a " or ',
		// if we don't get one then that's an error
		if (!(ch=skipWhitespace(getCharacter())) ||
					(ch!='"' && ch!='\'')) {
			parseAttributeFailed();
			return (char)NULL;
		}
	}

	// attribute values can be delimited by ' or "
	char	delimiter=ch;
	if (!(ch=getCharacter())) {
		return (char)NULL;
	}

	// get the attribute value
	data.clear();
	int	nest=0;
	for (;;) {

		if (standalone=='!' &&
			((delimiter=='[' && ch==']') ||
			(delimiter=='(' && ch==')'))) {

			// handle nesting in internal subsets
			if (nest) {
				nest--;
				data.append(ch);
			} else {
				break;
			}

		} else if (ch==delimiter) {

			// handle nesting in internal subsets
			if (standalone=='!' && delimiter=='[') {
				data.append('[');
				nest++;
			} else if (standalone=='!' && delimiter=='(') {
				data.append('(');
				nest++;
			} else {
				// if we got a matching " or ' then we've
				// gotten the entire value, terminate it
				// and break out
				break;
			}

		} else {

			if (ch=='&') {

				// handle general entities
				char	*buffer;
				int	result=getGeneralEntity(
						delimiter,&buffer);

				if (!result) {

					// if we hit the end, that's
					// an error
					parseAttributeFailed();
					return (char)NULL;

				} else if (result<0) {

					// if we hit the break character
					// (or an &) then write the
					// buffer to the value and loop
					// back without getting a new
					// character
					data.append(buffer);
					if (result==-1) {
						ch=delimiter;
					} else {
						ch='&';
					}
					delete[] buffer;
					continue;
				}

				// write the buffer to the value
				// and clean up
				data.append(buffer);
				delete[] buffer;

			} else {
				// add the character to the value
				data.append(ch);
			}
		}

		// get the next character
		if (!(ch=getCharacter())) {
			return (char)NULL;
		}
	}

	// call the callback for attribute
	if (!attributeValue(data.getString())) {
		return (char)NULL;
	}

	// return the first character after the attribute
	return getCharacter();
}

int xmlsax::getGeneralEntity(char breakchar, char **buffer) {

	// create a buffer and set the first character to &
	*buffer=new char[7];
	(*buffer)[0]='&';

	// get until a ; or the next 5 characters, whatever is smaller
	int	i;
	for (i=1; i<6; i++) {

		(*buffer)[i]=getCharacter();

		// jump out if we hit the end
		if (!(*buffer)[i]) {
			(*buffer)[i]=(char)NULL;
			return 0;
		}

		// if we find a break character, don't add it to the buffer,
		// just terminate and return the existing buffer
		if ((*buffer)[i]==breakchar) {
			(*buffer)[i]=(char)NULL;
			return -1;
		}

		// if we find a & then treat it similarly to a break character
		if ((*buffer)[i]=='&') {
			(*buffer)[i]=(char)NULL;
			return -2;
		}

		// if we find a ; then we're done
		if ((*buffer)[i]==';') {
			(*buffer)[i+1]=(char)NULL;
			break;
		}
	}

	// terminate the buffer if necessary
	if (i==6) {
		(*buffer)[6]=(char)NULL;
	}

	// handle some predefined general entities
	if (!charstring::compare((*buffer),"&amp;")) {
		(*buffer)[0]='&';
		(*buffer)[1]=(char)NULL;
	} else if (!charstring::compare((*buffer),"&lt;")) {
		(*buffer)[0]='<';
		(*buffer)[1]=(char)NULL;
	} else if (!charstring::compare((*buffer),"&gt;")) {
		(*buffer)[0]='>';
		(*buffer)[1]=(char)NULL;
	} else if (!charstring::compare((*buffer),"&apos;")) {
		(*buffer)[0]='\'';
		(*buffer)[1]=(char)NULL;
	} else if (!charstring::compare((*buffer),"&quot;")) {
		(*buffer)[0]='"';
		(*buffer)[1]=(char)NULL;
	} else {
		// handle numeric general entities
		if ((*buffer)[1]=='#') {
			long	number=atoi((*buffer)+2);
			if (number>127) {
				number=127;
			}
			(*buffer)[0]=(char)number;
			(*buffer)[1]=(char)NULL;
		}
	}

	return 1;
}

bool xmlsax::parseText(char current, char *next) {

	// create a buffer to hold the text
	stringbuffer	textdata;

	char	ch=current;
	for (;;) {

		if (!ch) {

			// we should not run into a NULL or EOF here, if we do
			// then return an error.
			*next=(char)NULL;
			return false;

		} else if (ch=='<') {

			// if we find an opening < then it should be a tag,
			// call the text callback and return the <
			text(textdata.getString());
			*next=ch;
			return true;

		}


		if (ch=='&') {

			// handle general entities
			char	*buffer;
			int	result=getGeneralEntity('<',&buffer);

			if (!result) {

				// if we hit the end, that's an error
				parseTextFailed();
				*next=(char)NULL;
				return false;

			} else if (result<0) {

				// if we hit the break character (or an &) then
				// write the buffer to the textdata and loop
				// back without getting a new character
				textdata.append(buffer);
				if (result==-1) {
					ch='<';
				} else {
					ch='&';
				}
				delete[] buffer;
				continue;
			}

			// write the buffer to the textdata and clean up
			textdata.append(buffer);
			delete[] buffer;

		} else {

			// if we haven't hit any exit conditions,
			// add the character to the buffer
			textdata.append(ch);
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::skipWhitespace(char current) {

	char	ch=current;
	int	first=1;

	for (;;) {

		// the first time, just process the current character
		if (!first) {
			ch=getCharacter();
		}

		// if we find a non-whitespace character, we need to break out
		// unless it's a NULL on the first time
		if (ch!=' ' && ch!='	' && ch!='\n' && ch!='\r') {
			if (!ch) {
				if (!first) {
					break;
				}
			} else {
				break;
			}
		}

		// indicate that it's no longer the first time
		if (first) {
			first=0;
		}
	}

	// return the first character after the whitespace
	return ch;
}

char xmlsax::getCharacter() {

	// get a character from the string or file, whichever is appropriate,
	// if the character is an EOF, return a NULL
	char	ch;
	if (string) {
		ch=*ptr;
		ptr++;
	} else {
		if (fl.read((void *)&ch,sizeof(char))!=sizeof(char)) {
			ch=(char)NULL;
		}
	}
	if (ch=='\n') {
		line++;
	}
	return ch;
}

void xmlsax::parseTagFailed() {
	clearError();
	appendError("error: parseTagFailed() failed at line ");
	appendError(line);
}

void xmlsax::parseAttributeFailed() {
	clearError();
	appendError("error: parseAttributeFailed() failed at line ");
	appendError(line);
}

void xmlsax::parseTextFailed() {
	clearError();
	appendError("error: parseText() failed at line ");
	appendError(line);
}
