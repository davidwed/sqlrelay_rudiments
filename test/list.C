#include <rudiments/list.h>
#include <stdio.h>

void	print(primitivelist<int> *l) {

	printf("length: %d\n",l->getLength());

	int	value;
	for (int i=0; i<l->getLength(); i++) {
		l->getValue(i,&value);
		printf("%d: %d,%d\n",i,l->getNode(i)->getValue(),value);
	}
}

void	print(stringlist *l) {

	printf("length: %d\n",l->getLength());

	char	*value;
	for (int i=0; i<l->getLength(); i++) {
		l->getValue(i,&value);
		printf("%d: %s,%s\n",i,l->getNode(i)->getValue(),value);
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
	printf("removeIndex(0)\n");
	prl.removeIndex(0);
	print(&prl);
	printf("removeValue(3)\n");
	prl.removeValue(3);
	print(&prl);
	printf("setValue(1,50)\n");
	prl.setValue(1,50);
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
	printf("removeIndex(0)\n");
	strl.removeIndex(0);
	print(&strl);
	printf("removeValue(three)\n");
	strl.removeValue("three");
	print(&strl);
	printf("setValue(1,fifty)\n");
	strl.setValue(1,"fifty");
	print(&strl);
	printf("clear()\n");
	strl.clear();
	print(&strl);
}
