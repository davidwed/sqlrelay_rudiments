// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmlsax.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/xmlsaxinlines.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

int xmlsax::parseFile(const char *filename) {

	// reset string/line
	reset();

	// close any previously opened files, open the file, parse it, close
	// it again
	return (close() && (filedescriptor=open(filename,O_RDONLY))>-1 &&
		parse() && close());
}

int xmlsax::parseString(const char *string) {

	// close any previously opened files
	if (!close()) {
		return 0;
	}

	// reset filedescriptor/line
	reset();

	// set string pointers
	this->string=(char *)string;
	ptr=this->string;

	return parse();
}

int xmlsax::close() {

	// close any previously opened files
	if (filedescriptor>-1 && ::close(filedescriptor)==-1) {
		return 0;
	}

	// reset string/filedescriptor/line
	reset();

	return 1;
}

int xmlsax::parse() {

	char	ch;

	// skip whitespace/check for an empty document
	if (!(ch=skipWhitespace((char)NULL))) {
		return 1;
	}

	// parse the document body
	for (;;) {

		// parse the tag
		ch=parseTag(ch);
		if (ch==-1) {
			return 0;
		} else if (!ch) {
			break;
		}

		// parse text until we find another tag
		ch=parseText(ch);
		if (ch==-1) {
			return 0;
		} else if (!ch) {
			break;
		}
	}

	// document parsed successfully
	return 1;
}

char xmlsax::parseTag(char current) {

	char	ch=current;

	// make sure there's a <, skip any whitespace after it
	if (ch!='<' || !(ch=skipWhitespace(getCharacter()))) {
		parseTagFailed();
		return -1;
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
			return -1;
		}
	}

	// get the tag name
	stringbuffer	*name;
	if (!(ch=parseTagName(ch,&name))) {
		delete name;
		parseTagFailed();
		return -1;
	}

	// handle comments and cdata
	if (!strcmp(name->getString(),"!--")) {
		if (!(ch=parseComment(ch))) {
			delete name;
			parseTagFailed();
			return -1;
		}
		delete name;
		return getCharacter();
	} else if (!strcmp(name->getString(),"![CDATA[")) {
		if (!(ch=parseCData(ch))) {
			delete name;
			parseTagFailed();
			return -1;
		}
		delete name;
		return getCharacter();
	}

	if (endtag) {

		// skip whitespace after the tag name and look for >
		if (!(ch=skipWhitespace(ch)) || ch!='>') {
			delete name;
			parseTagFailed();
			return -1;
		}

	} else {

		// call the callback for tag start
		if (!tagStart(name->getString())) {
			delete name;
			return -1;
		}

		// parse the attributes
		for (;;) {

			// skip any whitespace before the attribute
			if (!(ch=skipWhitespace(ch))) {
				delete name;
				parseTagFailed();
				return -1;
			}
	
			if (ch=='/') {
				// empty tag
				endtag=1;
				if (!(ch=skipWhitespace(getCharacter())) ||
								ch!='>') {
					delete name;
					parseTagFailed();
					return -1;
				}
				break;
			} else if (ch=='?') {
				// ? standalone tag, make sure there's a >
				// immediately following the ?
				if (!(ch=getCharacter()) || ch!='>') {
					delete name;
					parseTagFailed();
					return -1;
				}
				break;
			} else if (ch=='>') {
				// normal tag
				break;
			} else {
				if (!(ch=parseAttribute(ch,standalone))) {
					delete name;
					parseTagFailed();
					return -1;
				}
			}
		}
	}

	// if the tag was an empty or standalone tag,
	// call the callback for tag end
	if (endtag || standalone) {
		if (!tagEnd(name->getString())) {
			delete name;
			return -1;
		}
	}

	// clean up
	delete name;

	// return the first character after the closing >
	return getCharacter();
}

char xmlsax::parseTagName(char current, stringbuffer **name) {

	// create a buffer to hold the tag name
	*name=new stringbuffer();

	int	bracketcount=0;

	// get characters and put them in the buffer
	char	ch=current;
	for (;;) {

		if (ch<1) {

			// we should not run into a NULL or EOF here, if we
			// do then it's an error
			clearError();
			appendError("error: parseTagName() failed at line ");
			appendError(line);
			return -1;

		} else if (ch=='[') {

			(*name)->append(ch);

			// CDATA tags will have 2 brackets (![CDATA[)
			// if we've found 2 brackets, we're done
			bracketcount++;
			if (bracketcount==2) {
				// return the character after
				// the end of the name
				return getCharacter();
			}

		} else if (ch==' ' || ch=='	' ||
				ch=='\n' || ch=='\r' || ch=='/' || ch=='>') {

			// if we find whitespace, a / or a > then we're done
			// parsing the name

			// return the character after the end of the name
			return ch;
		} else {
			(*name)->append(ch);
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::parseComment(char current) {

	// create a buffer to store the comment
	stringbuffer	*text=new stringbuffer();
	char		ch=current;

	for (;;) {

		// handle potential terminators
		if (ch=='-') {
			if (!(ch=getCharacter())) {
				delete text;
				return (char)NULL;
			} else if (ch=='-') {
				if (!(ch=getCharacter())) {
					delete text;
					return (char)NULL;
				} else if (ch=='>') {
					// call the comment callback
					comment(text->getString());
					delete text;
					return ch;
				} else {
					text->append("--");
				}
			} else {
				text->append('-');
			}
		}

		text->append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			delete text;
			return (char)NULL;
		}
	}
}

char xmlsax::parseCData(char current) {

	// create a buffer to store the comment
	stringbuffer	*text=new stringbuffer();
	char		ch=current;
	int		nest=0;

	for (;;) {

		// handle potential terminators
		if (ch=='[') {
			nest++;
		} else if (ch==']') {
			if (nest==0) {
				if (!(ch=getCharacter())) {
					delete text;
					return (char)NULL;
				} else if (ch==']') {
					// call the cdata callback
					cdata(text->getString());
					delete text;
					break;
				} else {
					text->append(']');
				}
			} else {
				nest--;
			}
		}

		text->append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			delete text;
			return (char)NULL;
		}
	}

	// skip whitespace, get the next character and return it,
	// it should be a >
	if (!(ch=skipWhitespace(getCharacter())) || ch!='>') {
		delete text;
		return (char)NULL;
	}
	return ch;
}

char xmlsax::parseAttribute(char current, char standalone) {

	char		ch=current;
	stringbuffer	*data=new stringbuffer();

	if (standalone!='!' ||
			(standalone=='!' && ch!='"' && ch!='\'' && ch!='[')) {

		// get the attribute name
		for (;;) {

			if (ch==' ' || ch=='	' || ch=='\n' || ch=='\r') {

				// if we got whitespace, skip past it
				if (!(ch=skipWhitespace(ch))) {
					delete data;
					parseAttributeFailed();
					return (char)NULL;
				}
	
				if (standalone) {
					// for standalone tags, return an
					// attribute value,
					if (!attributeValue(
							data->getString())) {
						delete data;
						return (char)NULL;
					}
					return ch;
				} else {
					// for non-standalone, make sure there's
					// an = after the whitespace
					if (ch!='=') {
						delete data;
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
				data->append(ch);
				if (!(ch=getCharacter())) {
					delete data;
					return (char)NULL;
				}
			}
		}

		// call the attribute name callback
		if (!attributeName(data->getString())) {
			delete data;
			return (char)NULL;
		}

		// skip any whitespace after the =, then look for a " or ',
		// if we don't get one then that's an error
		if (!(ch=skipWhitespace(getCharacter())) ||
					(ch!='"' && ch!='\'')) {
			delete data;
			parseAttributeFailed();
			return (char)NULL;
		}
	}

	// attribute values can be delimited by ' or "
	char	delimiter=ch;
	if (!(ch=getCharacter())) {
		delete data;
		return (char)NULL;
	}

	// get the attribute value
	delete data;
	data=new stringbuffer();
	int	nest=0;
	for (;;) {

		if (standalone=='!' && delimiter=='[' && ch==']') {

			// handle nesting in internal subsets
			if (nest) {
				nest--;
				data->append(']');
			} else {
				break;
			}

		} else if (ch==delimiter) {

			// handle nesting in internal subsets
			if (standalone=='!' && delimiter=='[') {
				data->append('[');
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
					delete data;
					parseAttributeFailed();
					return (char)NULL;

				} else if (result<0) {

					// if we hit the break character
					// (or an &) then write the
					// buffer to the value and loop
					// back without getting a new
					// character
					data->append(buffer);
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
				data->append(buffer);
				delete[] buffer;

			} else {
				// add the character to the value
				data->append(ch);
			}
		}

		// get the next character
		if (!(ch=getCharacter())) {
			delete data;
			return (char)NULL;
		}
	}

	// call the callback for attribute
	if (!attributeValue(data->getString())) {
		delete data;
		return (char)NULL;
	}

	// clean up
	delete data;

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
	if (!strcmp((*buffer),"&amp;")) {
		(*buffer)[0]='&';
		(*buffer)[1]=(char)NULL;
	} else if (!strcmp((*buffer),"&lt;")) {
		(*buffer)[0]='<';
		(*buffer)[1]=(char)NULL;
	} else if (!strcmp((*buffer),"&gt;")) {
		(*buffer)[0]='>';
		(*buffer)[1]=(char)NULL;
	} else if (!strcmp((*buffer),"&apos;")) {
		(*buffer)[0]='\'';
		(*buffer)[1]=(char)NULL;
	} else if (!strcmp((*buffer),"&quot;")) {
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

char xmlsax::parseText(char current) {

	// create a buffer to hold the text
	stringbuffer	*textdata=new stringbuffer();

	char	ch=current;
	for (;;) {

		if (!ch) {
			return (char)NULL;
		}

		if (ch<1) {

			// we shouldn't get any NULL's or EOF's, if we do
			// then that's an error
			delete textdata;
			parseTextFailed();
			return -1;

		} else if (ch=='<') {

			// if we find an opening < then it should be a tag,
			// call the text callback and return the <
			text(textdata->getString());
			delete textdata;
			return ch;

		}


		if (ch=='&') {

			// handle general entities
			char	*buffer;
			int	result=getGeneralEntity('<',&buffer);

			if (!result) {

				// if we hit the end, that's an error
				parseTextFailed();
				delete textdata;
				return (char)NULL;

			} else if (result<0) {

				// if we hit the break character (or an &) then
				// write the buffer to the textdata and loop
				// back without getting a new character
				textdata->append(buffer);
				if (result==-1) {
					ch='<';
				} else {
					ch='&';
				}
				delete[] buffer;
				continue;
			}

			// write the buffer to the textdata and clean up
			textdata->append(buffer);
			delete[] buffer;

		} else {

			// if we haven't hit any exit conditions,
			// add the character to the buffer
			textdata->append(ch);
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
		ptr++;
		ch=*ptr;
	} else {
		if (read(filedescriptor,(void *)&ch,sizeof(char))!=
							sizeof(char)) {
			ch=(char)NULL;
		}
	}
	if (ch=='\n') {
		line++;
	}
	return ch;
}
