#include <rudiments/list.h>
#include <stdio.h>

void	print(primitivelist<int> *l) {

	printf("length: %d\n",l->getLength());

	int	value;
	for (int i=0; i<l->getLength(); i++) {
		l->getDataByIndex(i,&value);
		printf("%d: %d,%d\n",i,l->getNodeByIndex(i)->getData(),value);
	}
}

void	print(stringlist *l) {

	printf("length: %d\n",l->getLength());

	char	*value;
	for (int i=0; i<l->getLength(); i++) {
		l->getDataByIndex(i,&value);
		printf("%d: %s,%s\n",i,l->getNodeByIndex(i)->getData(),value);
	}
}

int	main(int argc, char **argv) {

	primitivelist<int>	prl;

	printf("append(0)\n");
	prl.append(0);
	print(&prl);
	printf("append(1)\n");
	prl.append(1);
	print(&prl);
	printf("append(3)\n");
	prl.append(3);
	print(&prl);
	printf("insert(2,2)\n");
	prl.insert(2,2);
	print(&prl);
	printf("removeByIndex(0)\n");
	prl.removeByIndex(0);
	print(&prl);
	printf("removeByKey(3)\n");
	prl.removeByKey(3);
	print(&prl);
	printf("setDataByIndex(1,50)\n");
	prl.setDataByIndex(1,50);
	print(&prl);
	printf("clear()\n");
	prl.clear();
	print(&prl);


	printf("\n\n");

	stringlist	strl;

	printf("append(zero)\n");
	strl.append("zero");
	print(&strl);
	printf("append(one)\n");
	strl.append("one");
	print(&strl);
	printf("append(three)\n");
	strl.append("three");
	print(&strl);
	printf("insert(2,two)\n");
	strl.insert(2,"two");
	print(&strl);
	printf("removeByIndex(0)\n");
	strl.removeByIndex(0);
	print(&strl);
	printf("removeByKey(three)\n");
	strl.removeByKey("three");
	print(&strl);
	printf("setDataByIndex(1,fifty)\n");
	strl.setDataByIndex(1,"fifty");
	print(&strl);
	printf("clear()\n");
	strl.clear();
	print(&strl);
}
