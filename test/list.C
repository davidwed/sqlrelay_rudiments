#include <rudiments/linkedlist.h>
#include <stdio.h>

template class linkedlist<int>;
template class linkedlistnode<int>;

void	print(linkedlist<int> *l) {

	printf("length: %d\n",l->getLength());

	int	value;
	for (int i=0; i<l->getLength(); i++) {
		l->getValue(i,&value);
		printf("%d: %d,%d\n",i,l->getNode(i)->getValue(),value);
	}
}

int	main(int argc, char **argv) {

	linkedlist<int>	l;

	printf("append(0)\n");
	l.append(0);
	print(&l);
	printf("append(1)\n");
	l.append(1);
	print(&l);
	printf("append(3)\n");
	l.append(3);
	print(&l);
	printf("insert(2,2)\n");
	l.insert(2,2);
	print(&l);
	printf("removeIndex(0)\n");
	l.removeIndex(0);
	print(&l);
	printf("removeValue(3)\n");
	l.removeValue(3);
	print(&l);
	printf("setValue(2,50)\n");
	l.setValue(1,50);
	print(&l);
	printf("clear()\n");
	l.clear();
	print(&l);
}
