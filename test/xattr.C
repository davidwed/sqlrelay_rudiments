#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {

	file	testfile;
	testfile.create("testfile",permissions::evalPermString("rw-r--r--"),
				"hello world\n");

	char	**attrs=testfile.listAttributes();
	if (attrs) {
		printf("Attributes:\n");
		for (int i=0; attrs[i]; i++) {
			unsigned char	*buffer;
			size_t		size;
			testfile.getAttribute(attrs[i],&buffer,&size);
			printf("%s = \"%s\"\n",attrs[i],buffer);
		}
	} else {
		printf("Error: %s\n",strerror(errno));
	}
	printf("-------------------------------\n");

	testfile.createAttribute("testattr1",
				(const unsigned char *)"testvalue1",10);
	testfile.createAttribute("testattr2",
				(const unsigned char *)"testvalue2",10);

	attrs=testfile.listAttributes();
	if (attrs) {
		printf("Attributes:\n");
		for (int i=0; attrs[i]; i++) {
			unsigned char	*buffer;
			size_t		size;
			testfile.getAttribute(attrs[i],&buffer,&size);
			printf("%s = \"%s\"\n",attrs[i],buffer);
		}
	} else {
		printf("Error: %s\n",strerror(errno));
	}
	printf("-------------------------------\n");

	testfile.replaceAttribute("testattr1",
				(const unsigned char *)"testval1",10);
	testfile.replaceAttribute("testattr2",
				(const unsigned char *)"testval2",10);

	attrs=testfile.listAttributes();
	if (attrs) {
		printf("Attributes:\n");
		for (int i=0; attrs[i]; i++) {
			unsigned char	*buffer;
			size_t		size;
			testfile.getAttribute(attrs[i],&buffer,&size);
			printf("%s = \"%s\"\n",attrs[i],buffer);
		}
	} else {
		printf("Error: %s\n",strerror(errno));
	}
	printf("-------------------------------\n");

	testfile.setAttribute("testattr1",(const unsigned char *)"testval1",10);
	testfile.setAttribute("testattr2",(const unsigned char *)"testval2",10);
	testfile.setAttribute("testattr3",(const unsigned char *)"testval3",10);
	testfile.setAttribute("testattr4",(const unsigned char *)"testval4",10);

	attrs=testfile.listAttributes();
	if (attrs) {
		printf("Attributes:\n");
		for (int i=0; attrs[i]; i++) {
			unsigned char	*buffer;
			size_t		size;
			testfile.getAttribute(attrs[i],&buffer,&size);
			printf("%s = \"%s\"\n",attrs[i],buffer);
		}
	} else {
		printf("Error: %s\n",strerror(errno));
	}
	printf("-------------------------------\n");

	testfile.removeAttribute("testattr1");
	testfile.removeAttribute("testattr2");
	testfile.removeAttribute("testattr3");
	testfile.removeAttribute("testattr4");

	attrs=testfile.listAttributes();
	if (attrs) {
		printf("Attributes:\n");
		for (int i=0; attrs[i]; i++) {
			unsigned char	*buffer;
			size_t		size;
			testfile.getAttribute(attrs[i],&buffer,&size);
			printf("%s = \"%s\"\n",attrs[i],buffer);
		}
	} else {
		printf("Error: %s\n",strerror(errno));
	}

	testfile.close();

	file::remove("testfile");
}
