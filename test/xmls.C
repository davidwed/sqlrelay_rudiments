// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/xmlsax.h>

#include <stdio.h>

class myxmlsax : public xmlsax {
	public:
			myxmlsax();
	private:
		int	xmlVersionStart();
		int	xmlVersionEnd();
		int	doctypeStart(char *name);
		int	externalSubset(char *filename);
		int	doctypeEnd();
		int	tagStart(char *name);
		int	attributeName(char *name);
		int	attributeValue(char *value);
		int	text(char *string);
		int	tagEnd(char *name);
		int	comment(char *string);
		int	cdata(char *string);
		void	indent(int spaces);
		int	ind;
};

myxmlsax::myxmlsax() : xmlsax() {
	ind=0;
}

int	myxmlsax::xmlVersionStart() {
	printf("XML version start:\n");
	return 1;
}

int	myxmlsax::xmlVersionEnd() {
	printf("XML version end:\n");
	return 1;
}

int	myxmlsax::doctypeStart(char *name) {
	printf("DOCTYPE start: %s\n",name);
	return 1;
}

int	myxmlsax::externalSubset(char *filename) {
	printf("	external subset: %s\n",filename);
	return 1;
}

int	myxmlsax::doctypeEnd() {
	printf("DOCTYPE end:\n");
	return 1;
}

void	myxmlsax::indent(int spaces) {
	for (int i=0; i<spaces; i++) {
		printf("  ");
	}
}

int	myxmlsax::tagStart(char *name) {
	indent(ind);
	printf("tagStart: %s\n",name);
	ind++;
}

int	myxmlsax::attributeName(char *name) {
	indent(ind+1);
	printf("attribute name: %s\n",name);
}

int	myxmlsax::attributeValue(char *value) {
	indent(ind+1);
	printf("attribute value: %s\n",value);
}

int	myxmlsax::text(char *string) {
	indent(ind+1);
	printf("text: \n%s\n",string);
}

int	myxmlsax::tagEnd(char *name) {
	ind--;
	indent(ind);
	printf("tagEnd: %s\n",name);
}

int	myxmlsax::comment(char *string) {
	indent(ind);
	printf("comment: \n%s\n",string);
}

int	myxmlsax::cdata(char *string) {
	indent(ind);
	printf("cdata: \n%s\n",string);
}


int	main() {

	myxmlsax	x;
	printf("=====================================================\n");
	x.parseString("
<?xml version=\"1.0\" ?>
<!DOCTYPE test SYSTEM \"test.dtd\" [ test [test [test [test]]]]>
<test>start test text
	<tag1 name11='&amp;&lt;&gt;&apos;&quot;' name12='&amp&lt&gt&apos&quot' name13='&&&&&' / > end tag1 text
	<tag2 name21=\"value21\" name22=\"value22\"> start tag2 text
		<tag3 name31=\"value31\" name32=\"value32\"> start tag3 text
			<tag4 name42=\"value41\" name42=\"value42\" /> end tag4 end text
		</tag3> end tag3 text
<!-- test comment -->
<?target test=\"test1\"?>
	</tag2> end tag2 text
&amp;&lt;&gt;&apos;&quot;
&amp&lt&gt&apos&quot
&&&&&
<![CDATA[ test [ test [ test [test]]]]]>
</test>
");
	printf("=====================================================\n");
	x.parseFile("xmls.xml");
}
