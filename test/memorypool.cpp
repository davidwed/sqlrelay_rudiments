// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/memorypool.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const uint32_t	stresscycles=1024*50;
const uint32_t	stresssize=4096;

int main(int argc, const char **argv) {

	header("memorypool");


	uint16_t	i;
	uint16_t	j;
	bool		success;
	memorypool	*mp;
	char		*segment[60];


	// characters...
	stdoutput.printf("char...\n");

	// first run
	mp=new memorypool(32,16,10);
	for (i=0; i<20; i++) {
		segment[i]=(char *)mp->allocate(6);
		for (j=0; j<6; j++) {
			segment[i][j]=(char)'a'+i;
		}
	}
	success=true;
	for (i=0; i<20 && success; i++) {
		for (j=0; j<6 && success; j++) {
			if (segment[i][j]!=(char)'a'+i) {
				success=false;
			}
		}
	}
	test("first run",success);
	mp->deallocate();

	// second run
	segment[0]=(char *)mp->allocate(40);
	for (j=0; j<40; j++) {
		segment[0][j]='z';
	}
	for (i=1; i<21; i++) {
		segment[i]=(char *)mp->allocate(6);
		for (j=0; j<6; j++) {
			segment[i][j]=(char)'a'+i-1;
		}
	}
	for (i=21; i<41; i++) {
		segment[i]=(char *)mp->allocate(2);
		for (j=0; j<2; j++) {
			segment[i][j]=(char)'A'+i-21;
		}
	}
	success=true;
	for (j=0; j<40 && success; j++) {
		if (segment[0][j]!='z') {
			success=false;
		}
	}
	test("second run (z)",success);
	for (i=1; i<21 && success; i++) {
		for (j=0; j<6 && success; j++) {
			if (segment[i][j]!=(char)'a'+i-1) {
				success=false;
			}
		}
	}
	test("second run (a)",success);
	for (i=21; i<41 && success; i++) {
		for (j=0; j<2 && success; j++) {
			if (segment[i][j]!=(char)'A'+i-21) {
				success=false;
			}
		}
	}
	test("second run (A)",success);
	mp->deallocate();
	stdoutput.printf("\n");


	// scalar numbers
	stdoutput.printf("scalar numbers...\n");
	double		*pd=(double *)mp->allocate(sizeof(double));
	float		*pf=(float *)mp->allocate(sizeof(float));
	uint64_t	*p64=(uint64_t *)mp->allocate(sizeof(uint64_t));
	uint32_t	*p32=(uint32_t *)mp->allocate(sizeof(uint32_t));
	uint16_t	*p16=(uint16_t *)mp->allocate(sizeof(uint16_t));
	*pd=2.3456;
	*pf=1.2345;
	*p64=3;
	*p32=2;
	*p16=1;
	test("double",(uint64_t)(*pd*10000)==23456);
	test("float",(uint64_t)(*pf*10000)==12345);
	test("uint64_t",*p64==3);
	test("uint32_t",*p32==2);
	test("uint16_t",*p16==1);
	mp->deallocate();
	stdoutput.printf("\n");


	// number arrays
	stdoutput.printf("number arrays...\n");
	double		*pda=(double *)mp->allocate(sizeof(double)*10);
	float		*pfa=(float *)mp->allocate(sizeof(float)*10);
	uint64_t	*p64a=(uint64_t *)mp->allocate(sizeof(uint64_t)*10);
	uint32_t	*p32a=(uint32_t *)mp->allocate(sizeof(uint32_t)*10);
	uint16_t	*p16a=(uint16_t *)mp->allocate(sizeof(uint16_t)*10);
	for (i=0; i<10; i++) {
		pda[i]=2.3456;
		pfa[i]=1.2345;
		p64a[i]=i;
		p32a[i]=i;
		p16a[i]=i;
	}

	success=true;
	for (i=0; i<10 && success; i++) {
		if ((uint64_t)(pda[i]*10000)!=23456) {
			success=false;
		}
	}
	test("double",success);

	success=true;
	for (i=0; i<10 && success; i++) {
		if ((uint64_t)(pfa[i]*10000)!=12345) {
			success=false;
		}
	}
	test("float",success);

	success=true;
	for (i=0; i<10 && success; i++) {
		if (p64a[i]!=i) {
			success=false;
		}
	}
	test("uint64_t",success);

	success=true;
	for (i=0; i<10 && success; i++) {
		if (p32a[i]!=i) {
			success=false;
		}
	}
	test("uint32_t",success);

	success=true;
	for (i=0; i<10 && success; i++) {
		if (p16a[i]!=i) {
			success=false;
		}
	}
	test("uint16_t",success);

	mp->deallocate();
	delete mp;
	stdoutput.printf("\n");


	// stress
	stdoutput.printf("stress...\n");

	// allocate from the pool, deallocating after each cycle...
	mp=new memorypool(32,16,10);
	datetime	dt;
	dt.getSystemDateAndTime();
	time_t	start=dt.getEpoch();
	for (i=0; i<stresscycles; i++) {
		for (j=0; j<stresssize; j++) {
			mp->allocate(j);
		}
		mp->deallocate();
	}
	dt.getSystemDateAndTime();
	time_t	end=dt.getEpoch();
	delete mp;
	time_t	memorypooltime=end-start;
	stdoutput.printf("	memory pool time: %d\n",memorypooltime);

	// ...as opposed to allocating and deallocating on-demand
	dt.getSystemDateAndTime();
	start=dt.getEpoch();
	for (i=0; i<stresscycles; i++) {
		for (j=0; j<stresssize; j++) {
			char	*data=new char[j];
			delete[] data;
		}
	}
	dt.getSystemDateAndTime();
	end=dt.getEpoch();
	time_t	ondemandtime=end-start;
	stdoutput.printf("	on-demand time: %d\n",ondemandtime);

	// and as opposed to allocating on-demand, keeping a chunk-list,
	// and deallocating after each cycle...
	char	*chunklist[stresssize];
	dt.getSystemDateAndTime();
	start=dt.getEpoch();
	for (i=0; i<stresscycles; i++) {
		for (j=0; j<stresssize; j++) {
			chunklist[j]=new char[j];
		}
		for (j=0; j<stresssize; j++) {
			delete[] chunklist[j];
		}
	}
	dt.getSystemDateAndTime();
	end=dt.getEpoch();
	time_t	chunklisttime=end-start;
	stdoutput.printf("	chunk list time: %d\n",chunklisttime);
	
	test("faster than on-demand",ondemandtime>memorypooltime);
	test("faster than chunk-list",chunklisttime>memorypooltime);

	stdoutput.printf("\n");
}
