#include <rudiments/list.h>
#include <stdio.h>

int	main(int argc, char **argv) {

	list<int>	prl;

	printf("append(0)\n");
	prl.append(0);
	prl.print();
	printf("append(1)\n");
	prl.append(1);
	prl.print();
	printf("append(3)\n");
	prl.append(3);
	prl.print();
	printf("insert(2,2)\n");
	prl.insert(2,2);
	prl.print();
	printf("removeByIndex(0)\n");
	prl.removeByIndex(0);
	prl.print();
	printf("removeByData(3)\n");
	prl.removeByData(3);
	prl.print();
	printf("setDataByIndex(1,50)\n");
	prl.setDataByIndex(1,50);
	prl.print();
	printf("clear()\n");
	prl.clear();
	prl.print();


	printf("\n\n");

	stringlist	strl;

	printf("append(zero)\n");
	strl.append("zero");
	strl.print();
	printf("append(one)\n");
	strl.append("one");
	strl.print();
	printf("append(three)\n");
	strl.append("three");
	strl.print();
	printf("insert(2,two)\n");
	strl.insert(2,"two");
	strl.print();
	printf("removeByIndex(0)\n");
	strl.removeByIndex(0);
	strl.print();
	printf("removeByData(three)\n");
	strl.removeByData("three");
	strl.print();
	printf("setDataByIndex(1,fifty)\n");
	strl.setDataByIndex(1,"fifty");
	strl.print();
	printf("clear()\n");
	strl.clear();
	strl.print();
}
