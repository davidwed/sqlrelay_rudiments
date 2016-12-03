// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/memorypool.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

const uint32_t	stresscycles=1024*50;
const uint32_t	stresssize=4096;

int main(int argc, const char **argv) {

	uint32_t	i;
	uint32_t	j;

	// stress
	stdoutput.printf("stress...\n");

	// allocate from the pool, deallocating after each cycle...
	stdoutput.printf("	memory pool... ");
	memorypool	*mp=new memorypool(32,16,10);
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

	stdoutput.printf("%d seconds\n",memorypooltime);


	// ...as opposed to allocating and deallocating on-demand
	stdoutput.printf("	on-demand...   ");
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

	// should be about 3 times slower on modern Linux
	// and about 10 times slower on modern Windows
	stdoutput.printf("%d seconds\n",ondemandtime);

#ifdef _WIN32
	stdoutput.printf("(this next one might take a while)\n");
#endif

	// ...and as opposed to allocating on-demand, keeping a chunk-list,
	// and deallocating all chunks after each cycle
	stdoutput.printf("	chunk-list...  ");
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

	// should be about 3 times slower on modern Linux
	// and about 200 times slower on modern Windows
	stdoutput.printf("%d seconds\n",chunklisttime);

	stdoutput.printf("\n");
}
