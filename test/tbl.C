#include <rudiments/table.h>
#include <stdio.h>
#include <string.h>

int main(int argv, const char **argc) {

	table	tbl;

	tbl.initialize(5,5,1);

	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			char	string[10];
			sprintf(string,"%d,%d",i,j);
			tbl.setFieldData(i,j,(unsigned char *)string,
						strlen(string));
		}
	}


	printf("\ntable: %dx%d\n",tbl.getRowCount(),tbl.getColumnCount());
	unsigned char	***fields=tbl.getFields();
	for (int i=0; i<tbl.getRowCount(); i++) {
		for (int j=0; j<tbl.getColumnCount(); j++) {
			printf("%s, ",fields[i][j]);
		}
	}
}
