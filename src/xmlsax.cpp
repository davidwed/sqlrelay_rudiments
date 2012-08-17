// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmlsax.h>
#include <rudiments/charstring.h>
#include <rudiments/filesystem.h>
#include <rudiments/system.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmlsaxprivate {
	friend class xmlsax;
	private:
		const char	*_string;
		const char	*_ptr;
		const char	*_endptr;
		file		_fl;
		bool		_mmapped;
		off64_t		_filesize;
		off64_t		_optblocksize;
		#ifdef RUDIMENTS_HAVE_MMAP
		memorymap	_mm;
		off64_t		_fileoffset;
		#endif
		uint32_t	_line;
		stringbuffer	_err;

		// reusing these over and over reduces heap fragmentation
		stringbuffer	_tagname;
		stringbuffer	_commenttext;
		stringbuffer	_cdatatext;
		stringbuffer	_attrdata;
		stringbuffer	_textdata;
		char		_entitybuffer[7];
};

xmlsax::xmlsax() {
	pvt=new xmlsaxprivate;
	reset();
}

xmlsax::~xmlsax() {
	close();
	delete pvt;
}

void xmlsax::reset() {
	pvt->_string=NULL;
	pvt->_ptr=NULL;
	pvt->_endptr=NULL;
	pvt->_filesize=0;
#ifdef RUDIMENTS_HAVE_MMAP
	pvt->_fileoffset=0;
#endif
	pvt->_mmapped=false;
	pvt->_line=1;
}

bool xmlsax::tagStart(const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeName(const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::attributeValue(const char *value) {
	// by default, just return success
	return true;
}

bool xmlsax::text(const char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::tagEnd(const char *name) {
	// by default, just return success
	return true;
}

bool xmlsax::comment(const char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::cdata(const char *string) {
	// by default, just return success
	return true;
}

bool xmlsax::parseFile(const char *filename) {

	// reset string/line
	reset();

	// close any previously opened files, open the file, parse it, close
	// it again
	close();

	// open the file
	bool retval;
	if ((retval=pvt->_fl.open(filename,O_RDONLY))) {

		// Set the read buffer size...

		// Get the optimum transfer size for the filesystem
		// the file is found on, if possible.
		filesystem	fs;
		pvt->_optblocksize=(fs.initialize(filename))?
					fs.getOptimumTransferBlockSize():1024;

		// If we're memory mapping, since we'll use this for the
		// offsets as well, then we must use an even multiple of
		// the memory page size.  Use the page size unless the 
		// transfer size is an even multiple of it.
		#ifdef RUDIMENTS_HAVE_MMAP
			off64_t	pagesize=system::getPageSize();
			if (pagesize>pvt->_optblocksize ||
				pvt->_optblocksize%pagesize) {
				pvt->_optblocksize=pagesize;
			}
		#endif

		// optimize...
		pvt->_fl.setReadBufferSize(pvt->_optblocksize);
		pvt->_fl.sequentialAccess(0,pvt->_filesize);
		pvt->_fl.onlyOnce(0,pvt->_filesize);

		// get the file size
		pvt->_filesize=pvt->_fl.getSize();

		// Try to memorymap the file.  If it fails, that's ok, pvt->_ptr
		// will be set to NULL from the previous call to reset() and
		// will cause getCharacter() to read from the file rather than
		// the map when parse() calls it.
		#ifdef RUDIMENTS_HAVE_MMAP
			pvt->_fileoffset=0;
			pvt->_mmapped=true;
			mapFile();
		#endif

		// parse the file
		retval=parse();

		// unmap the file, if necessary
		#ifdef RUDIMENTS_HAVE_MMAP
			if (pvt->_ptr) {
				pvt->_mm.detach();
			}
		#endif
	}

	// close and return
	close();
	return retval;
}

bool xmlsax::parseString(const char *string) {

	// close any previously opened files
	close();

	// reset fd/line
	reset();

	// set string pointers
	pvt->_ptr=pvt->_string=string;
	pvt->_endptr=pvt->_string+charstring::length(string);
	pvt->_mmapped=false;

	return parse();
}

void xmlsax::close() {

	// reset string/fd/line
	reset();

	// close any previously opened files
	pvt->_fl.close();
}

bool xmlsax::parse() {

	char	ch;

	// skip whitespace/check for an empty document
	if (!(ch=skipWhitespace('\0'))) {
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
	int32_t	endtag=0;
	char	standalone='\0';
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
	pvt->_tagname.clear();
	if (!parseTagName(ch,&pvt->_tagname,&ch)) {
		parseTagFailed();
		return false;
	}

	// handle comments and cdata
	if (!charstring::compare(pvt->_tagname.getString(),"!--")) {
		if (!(ch=parseComment(ch))) {
			parseTagFailed();
			return false;
		}
		return (*next=getCharacter())!='\0';
	} else if (!charstring::compare(pvt->_tagname.getString(),"![CDATA[")) {
		if (!(ch=parseCData(ch))) {
			parseTagFailed();
			return false;
		}
		return (*next=getCharacter())!='\0';
	}

	if (endtag) {

		// skip whitespace after the tag name and look for >
		if (!(ch=skipWhitespace(ch)) || ch!='>') {
			parseTagFailed();
			return false;
		}

	} else {

		// call the callback for tag start
		if (!tagStart(pvt->_tagname.getString())) {
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
		if (!tagEnd(pvt->_tagname.getString())) {
			return false;
		}
	}

	// return the first character after the closing >
	*next=getCharacter();
	return true;
}

bool xmlsax::parseTagName(char current, stringbuffer *name, char *next) {

	int32_t	bracketcount=0;

	// get characters and put them in the buffer
	char	ch=current;
	for (;;) {

		if (!ch) {

			// we should not run into a NULL or EOF here, if we
			// do then it's an error
			pvt->_err.clear();
			pvt->_err.append("error: parseTagName() ");
			pvt->_err.append("failed at line ");
			pvt->_err.append(pvt->_line);
			return false;

		} else if (ch=='[') {

			name->append(ch);

			// CDATA tags will have 2 brackets (![CDATA[)
			// if we've found 2 brackets, we're done
			bracketcount++;
			if (bracketcount==2) {
				// return the character after
				// the end of the name
				return (*next=getCharacter())!='\0';
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
			return (*next=getCharacter())!='\0';
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::parseComment(char current) {

	// create a buffer to store the comment
	pvt->_commenttext.clear();
	char		ch=current;

	for (;;) {

		// handle potential terminators
		if (ch=='-') {
			if (!(ch=getCharacter())) {
				return '\0';
			} else if (ch=='-') {
				if (!(ch=getCharacter())) {
					return '\0';
				} else if (ch=='>') {
					// call the comment callback
					comment(pvt->_commenttext.getString());
					return ch;
				} else {
					pvt->_commenttext.append("--");
				}
			} else {
				pvt->_commenttext.append('-');
			}
		}

		pvt->_commenttext.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			return '\0';
		}
	}
}

char xmlsax::parseCData(char current) {

	// create a buffer to store the comment
	pvt->_cdatatext.clear();
	char		ch=current;
	int32_t		nest=0;

	for (;;) {

		// handle potential terminators
		if (ch=='[') {
			nest++;
		} else if (ch==']') {
			if (nest==0) {
				if (!(ch=getCharacter())) {
					return '\0';
				} else if (ch==']') {
					// call the cdata callback
					cdata(pvt->_cdatatext.getString());
					break;
				} else {
					pvt->_cdatatext.append(']');
				}
			} else {
				nest--;
			}
		}

		pvt->_cdatatext.append(ch);

		// get the next character
		if (!(ch=getCharacter())) {
			return '\0';
		}
	}

	// skip whitespace, get the next character and return it,
	// it should be a >
	if (!(ch=skipWhitespace(getCharacter())) || ch!='>') {
		return '\0';
	}
	return ch;
}

char xmlsax::parseAttribute(char current, char standalone) {

	char		ch=current;
	pvt->_attrdata.clear();

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
					return '\0';
				}
	
				if (standalone) {
					// for standalone tags, return an
					// attribute value,
					if (!attributeValue(
						pvt->_attrdata.getString())) {
						return '\0';
					}
					return ch;
				} else {
					// for non-standalone, make sure there's
					// an = after the whitespace
					if (ch!='=') {
						parseAttributeFailed();
						return '\0';
					}
				}
	
			} else if (ch=='=') {
	
				// if we got an = then we've gotten the entire
				// name, terminate it and break out of the loop
				break;

			} else {
	
				// otherwise add the character
				// to the attribute name
				pvt->_attrdata.append(ch);
				if (!(ch=getCharacter())) {
					return '\0';
				}
			}
		}

		// call the attribute name callback
		if (!attributeName(pvt->_attrdata.getString())) {
			return '\0';
		}

		// skip any whitespace after the =, then look for a " or ',
		// if we don't get one then that's an error
		if (!(ch=skipWhitespace(getCharacter())) ||
					(ch!='"' && ch!='\'')) {
			parseAttributeFailed();
			return '\0';
		}
	}

	// attribute values can be delimited by ' or "
	char	delimiter=ch;
	if (!(ch=getCharacter())) {
		return '\0';
	}

	// get the attribute value
	pvt->_attrdata.clear();
	int32_t	nest=0;
	for (;;) {

		if (standalone=='!' &&
			((delimiter=='[' && ch==']') ||
			(delimiter=='(' && ch==')'))) {

			// handle nesting in internal subsets
			if (nest) {
				nest--;
				pvt->_attrdata.append(ch);
			} else {
				break;
			}

		} else if (ch==delimiter) {

			// handle nesting in internal subsets
			if (standalone=='!' && delimiter=='[') {
				pvt->_attrdata.append('[');
				nest++;
			} else if (standalone=='!' && delimiter=='(') {
				pvt->_attrdata.append('(');
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
				int32_t	result=getGeneralEntity(delimiter);

				if (!result) {

					// if we hit the end, that's
					// an error
					parseAttributeFailed();
					return '\0';

				} else if (result<0) {

					// if we hit the break character
					// (or an &) then write the
					// buffer to the value and loop
					// back without getting a new
					// character
					pvt->_attrdata.append(
						pvt->_entitybuffer);
					if (result==-1) {
						ch=delimiter;
					} else {
						ch='&';
					}
					continue;
				}

				// write the buffer to the value
				// and clean up
				pvt->_attrdata.append(pvt->_entitybuffer);

			} else {
				// add the character to the value
				pvt->_attrdata.append(ch);
			}
		}

		// get the next character
		if (!(ch=getCharacter())) {
			return '\0';
		}
	}

	// call the callback for attribute
	if (!attributeValue(pvt->_attrdata.getString())) {
		return '\0';
	}

	// return the first character after the attribute
	return getCharacter();
}

int32_t xmlsax::getGeneralEntity(char breakchar) {

	// create a buffer and set the first character to &
	pvt->_entitybuffer[0]='&';

	// get until a ; or the next 5 characters, whatever is smaller
	int32_t	i;
	for (i=1; i<6; i++) {

		pvt->_entitybuffer[i]=getCharacter();

		// jump out if we hit the end
		if (!pvt->_entitybuffer[i]) {
			pvt->_entitybuffer[i]='\0';
			return 0;
		}

		// if we find a break character, don't add it to the buffer,
		// just terminate and return the existing buffer
		if (pvt->_entitybuffer[i]==breakchar) {
			pvt->_entitybuffer[i]='\0';
			return -1;
		}

		// if we find a & then treat it similarly to a break character
		if (pvt->_entitybuffer[i]=='&') {
			pvt->_entitybuffer[i]='\0';
			return -2;
		}

		// if we find a ; then we're done
		if (pvt->_entitybuffer[i]==';') {
			pvt->_entitybuffer[i+1]='\0';
			break;
		}
	}

	// terminate the buffer if necessary
	if (i==6) {
		pvt->_entitybuffer[6]='\0';
	}

	// handle some predefined general entities
	if (!charstring::compare(pvt->_entitybuffer,"&amp;")) {
		pvt->_entitybuffer[0]='&';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&lt;")) {
		pvt->_entitybuffer[0]='<';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&gt;")) {
		pvt->_entitybuffer[0]='>';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&apos;")) {
		pvt->_entitybuffer[0]='\'';
		pvt->_entitybuffer[1]='\0';
	} else if (!charstring::compare(pvt->_entitybuffer,"&quot;")) {
		pvt->_entitybuffer[0]='"';
		pvt->_entitybuffer[1]='\0';
	} else {
		// handle numeric general entities
		if (pvt->_entitybuffer[1]=='#') {
			int64_t	number=charstring::toInteger(
							pvt->_entitybuffer+2);
			if (number>255) {
				number=255;
			}
			pvt->_entitybuffer[0]=static_cast<char>(number);
			pvt->_entitybuffer[1]='\0';
		}
	}

	return 1;
}

bool xmlsax::parseText(char current, char *next) {

	pvt->_textdata.clear();

	char	ch=current;
	for (;;) {

		if (!ch) {

			// we should not run into a NULL or EOF here, if we do
			// then return an error.
			*next='\0';
			return false;

		} else if (ch=='<') {

			// if we find an opening < then it should be a tag,
			// call the text callback and return the <
			text(pvt->_textdata.getString());
			*next=ch;
			return true;

		}


		if (ch=='&') {

			// handle general entities
			int32_t	result=getGeneralEntity('<');

			if (!result) {

				// if we hit the end, that's an error
				parseTextFailed();
				*next='\0';
				return false;

			} else if (result<0) {

				// if we hit the break character (or an &) then
				// write the buffer to the textdata and loop
				// back without getting a new character
				pvt->_textdata.append(pvt->_entitybuffer);
				if (result==-1) {
					ch='<';
				} else {
					ch='&';
				}
				continue;
			}

			// write the buffer to the textdata and clean up
			pvt->_textdata.append(pvt->_entitybuffer);

		} else {

			// if we haven't hit any exit conditions,
			// add the character to the buffer
			pvt->_textdata.append(ch);
		}

		// get the next character
		ch=getCharacter();
	}
}

char xmlsax::skipWhitespace(char current) {

	char	ch=current;
	int32_t	first=1;

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
	if (pvt->_string) {
		// If you've come here chasing valgrind errors...
		// ptr may be set to the return value of mmap() which is
		// neither on the stack nor in the heap.  There's no actual
		// error here, valgrind just doesn't know about variables that
		// aren't on the stack or in the heap and it thinks it's
		// uninitialized.
		if (pvt->_ptr==pvt->_endptr) {
			// if we're not parsing a memory-mapped file, we're done
			// if we're parsing a memory-mapped file,
			// we need to try to re-map it, if we can't we're done
			if (!pvt->_mmapped || !mapFile()) {
				return '\0';
			}
		}
		ch=*(pvt->_ptr);
		(pvt->_ptr)++;
		#ifdef RUDIMENTS_HAVE_MMAP
		if (pvt->_mmapped) {
			pvt->_fileoffset++;
		}
		#endif
	} else {
		if (pvt->_fl.read(&ch)!=sizeof(char)) {
			return '\0';
		}
	}
	if (ch=='\n') {
		(pvt->_line)++;
	}
	return ch;
}

void xmlsax::parseTagFailed() {
	pvt->_err.clear();
	pvt->_err.append("error: parseTagFailed() failed at line ");
	pvt->_err.append(pvt->_line);
}

void xmlsax::parseAttributeFailed() {
	pvt->_err.clear();
	pvt->_err.append("error: parseAttributeFailed() failed at line ");
	pvt->_err.append(pvt->_line);
}

void xmlsax::parseTextFailed() {
	pvt->_err.clear();
	pvt->_err.append("error: parseText() failed at line ");
	pvt->_err.append(pvt->_line);
}

const char *xmlsax::getError() {
	return pvt->_err.getString();
}


bool xmlsax::mapFile() {

#ifdef RUDIMENTS_HAVE_MMAP
	if (pvt->_fileoffset) {
		pvt->_mm.detach();
	}

	off64_t	len=pvt->_filesize-pvt->_fileoffset;
	if (len>pvt->_optblocksize) {
		len=pvt->_optblocksize;
	}
	if (!len) {
		return false;
	}

	if (pvt->_mm.attach(pvt->_fl.getFileDescriptor(),
				pvt->_fileoffset,len,PROT_READ,MAP_PRIVATE)) {
		pvt->_string=static_cast<char *>(pvt->_mm.getData());
		pvt->_ptr=pvt->_string;	
		pvt->_endptr=pvt->_ptr+len;
		return true;
	}
#endif
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
