// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/memorypool.h>
#include <stdio.h>

int main(int argv, const char **argc) {

	memorypool	*mp=new memorypool(32,16,10);

	printf("first run...\n");
	for (int i=0; i<20; i++) {
		char	*segment=(char *)mp->malloc(6);
		for (int j=0; j<6; j++) {
			segment[j]=(char)'a'+i;
		}
	}
	mp->print();

	printf("free...\n");
	mp->free();
	mp->print();

	printf("second run...\n");
	char	*segment=(char *)mp->malloc(40);
	for (int j=0; j<40; j++) {
		segment[j]='z';
	}
	for (int i=0; i<20; i++) {
		char	*segment=(char *)mp->malloc(6);
		for (int j=0; j<6; j++) {
			segment[j]=(char)'a'+i;
		}
	}
	for (int i=0; i<20; i++) {
		char	*segment=(char *)mp->malloc(2);
		for (int j=0; j<6; j++) {
			segment[j]=(char)'A'+i;
		}
	}
	mp->print();

	printf("free...\n");
	mp->free();
	mp->print();

	printf("short/long/float/double...\n");

	short	*sp=(short *)mp->malloc(sizeof(short));
	long	*lp=(long *)mp->malloc(sizeof(long));
	float	*fp=(float *)mp->malloc(sizeof(float));
	double	*dp=(double *)mp->malloc(sizeof(double));
	*sp=1;
	*lp=1;
	*fp=1.1;
	*dp=1.1;

	printf("sp: %d  lp: %d ",*sp,*lp);
	printf("fp: %f  dp: %f \n",*fp,*dp);
	mp->print();

	printf("free...\n");
	mp->free();
	mp->print();

	printf("short/long/float/double arrays...\n");

	short	*spa=(short *)mp->malloc(sizeof(short)*10);
	long	*lpa=(long *)mp->malloc(sizeof(long)*10);
	float	*fpa=(float *)mp->malloc(sizeof(float)*10);
	double	*dpa=(double *)mp->malloc(sizeof(double)*10);
	for (int i=0; i<10; i++) {
		spa[i]=i;
		lpa[i]=i;
		fpa[i]=i+((float)i/10.0);
		dpa[i]=i+((float)i/10.0);
	}

	for (int i=0; i<10; i++) {
		printf("sp: %d  lp: %d ",spa[i],lpa[i]);
		printf("fp: %f  dp: %f \n",fpa[i],dpa[i]);
	}
	mp->print();

	printf("free...\n");
	mp->free();
	mp->print();

	delete mp;


	mp=new memorypool(32,16,10);
	for (int i=0; i<10; i++) {
		unsigned long	total=0;
		for (unsigned long j=0; j<11; j++) {
			total=total+i+j;
			mp->malloc(i+j);
			mp->free();
		}
		printf("should be: %d\n",total/11);
		mp->print();
	}

	delete mp;
}
