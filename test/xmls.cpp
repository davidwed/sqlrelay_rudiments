// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmlsax.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

class myxmlsax : public xmlsax {
	public:
				myxmlsax();
		const char	*getString();
	private:
		bool	xmlVersionStart();
		bool	xmlVersionEnd();
		bool	doctypeStart(const char *name);
		bool	externalSubset(const char *filename);
		bool	doctypeEnd();
		bool	tagStart(const char *name);
		bool	attributeName(const char *name);
		bool	attributeValue(const char *value);
		bool	text(const char *string);
		bool	tagEnd(const char *name);
		bool	comment(const char *string);
		bool	cdata(const char *string);
		void	indent(int spaces);
		int	ind;

		stringbuffer	str;
};

myxmlsax::myxmlsax() : xmlsax() {
	ind=0;
}

bool	myxmlsax::xmlVersionStart() {
	str.append("XML version start:\n");
	return true;
}

bool	myxmlsax::xmlVersionEnd() {
	str.append("XML version end:\n");
	return true;
}

bool	myxmlsax::doctypeStart(const char *name) {
	str.append("DOCTYPE start: ")->append(name)->append("\n");
	return true;
}

bool	myxmlsax::externalSubset(const char *filename) {
	str.append("	external subset: ")->append(filename)->append("\n");
	return true;
}

bool	myxmlsax::doctypeEnd() {
	str.append("DOCTYPE end:\n");
	return true;
}

void	myxmlsax::indent(int spaces) {
	for (int i=0; i<spaces; i++) {
		str.append("  ");
	}
}

bool	myxmlsax::tagStart(const char *name) {
	indent(ind);
	str.append("tagStart: ")->append(name)->append("\n");
	ind++;
	return true;
}

bool	myxmlsax::attributeName(const char *name) {
	indent(ind+1);
	str.append("attribute name: ")->append(name)->append("\n");
	return true;
}

bool	myxmlsax::attributeValue(const char *value) {
	indent(ind+1);
	str.append("attribute value: ")->append(value)->append("\n");
	return true;
}

bool	myxmlsax::text(const char *string) {
	indent(ind+1);
	str.append("text: \n")->append(string)->append("\n");
	return true;
}

bool	myxmlsax::tagEnd(const char *name) {
	ind--;
	indent(ind);
	str.append("tagEnd: ")->append(name)->append("\n");
	return true;
}

bool	myxmlsax::comment(const char *string) {
	indent(ind);
	str.append("comment: \n")->append(string)->append("\n");
	return true;
}

bool	myxmlsax::cdata(const char *string) {
	indent(ind);
	str.append("cdata: \n")->append(string)->append("\n");
	return true;
}

const char	*myxmlsax::getString() {
	return str.getString();
}


int main(int argc, const char **argv) {

	myxmlsax	x;
	printf("=====================================================\n");
	x.parseString("\n\
<?xml version=\"1.0\" ?>\n\
<!DOCTYPE test SYSTEM \"test.dtd\" [ test [test [test [test]]]]>\n\
<test>start test text\n\
	<tag1 name11='&amp;&lt;&gt;&apos;&quot;' name12='&amp&lt&gt&apos&quot' name13='&&&&&' / > end tag1 text\n\
	<tag2 name21=\"value21\" name22=\"value22\"> start tag2 text\n\
		<tag3 name31=\"value31\" name32=\"value32\"> start tag3 text\n\
			<tag4 name42=\"value41\" name42=\"value42\" /> end tag4 end text\n\
		</tag3> end tag3 text\n\
<!-- test comment -->\n\
<?target test=\"test1\"?>\n\
	</tag2> end tag2 text\n\
&amp;&lt;&gt;&apos;&quot;\n\
&amp&lt&gt&apos&quot\n\
&&&&&\n\
<![CDATA[ test [ test [ test [test]]]]]>\n\
</test>\n\
");
	printf("=====================================================\n");
	x.parseFile("xmls.xml");
	printf("%s\n",x.getString());
}