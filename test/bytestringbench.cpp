// Copyright (c) 2017  David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_G_CONFIG_H
	#include <_G_config.h>
#endif

#define ITERATIONS 2048
#define BUFFERSIZE (4*1024*1024)

void *copy(void *dest, const void *src, size_t size) {
	return (dest && src)?memcpy(dest,src,size):NULL;
}

void *copy8(void *dest, const void *src, size_t size) {
	if (dest && src) {
		switch (size) {
			case 1:
			case 2:
				return memcpy(dest,src,size);
			case 4:
				if (!((((uint64_t)dest)%4)&
						(((uint64_t)src)%4))) {
					*((uint32_t *)dest)=*((uint32_t *)src);
					return dest;
				}
				break;
			case 8:
				if (!((((uint64_t)dest)%8)&
						(((uint64_t)src)%8))) {
					*((uint64_t *)dest)=*((uint64_t *)src);
					return dest;
				}
				break;
		}
		return memcpy(dest,src,size);
	}
	return NULL;
}

void displayTime(datetime *start, datetime *end) {
	uint32_t	sec=end->getEpoch()-start->getEpoch();
	int32_t		usec=end->getMicroseconds()-start->getMicroseconds();
	if (usec<0) {
		sec--;
		usec=usec+100000;
	}
	long double	totalsec=(long double)sec+
					(((long double)usec)/1000000.0);
	stdoutput.printf("%Lf\n",totalsec);
}

int main() {

	unsigned char	bytes[BUFFERSIZE];

	datetime	start;
	datetime	end;

	stdoutput.printf("uint8_t\n");
	stdoutput.printf("	copy  in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	uint8_t	val8=0xFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val8)); i++) {
			copy(bytes+(i*sizeof(val8)),&val8,sizeof(val8));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	val8=0xFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val8)); i++) {
			copy8(bytes+(i*sizeof(val8)),&val8,sizeof(val8));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy  out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val8)); i++) {
			copy(&val8,bytes+(i*sizeof(val8)),sizeof(val8));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val8)); i++) {
			copy8(&val8,bytes+(i*sizeof(val8)),sizeof(val8));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);



	stdoutput.printf("uint16_t\n");
	stdoutput.printf("	copy  in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	uint16_t	val16=0xFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val16)); i++) {
			copy(bytes+(i*sizeof(val16)),&val16,sizeof(val16));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	val16=0xFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val16)); i++) {
			copy8(bytes+(i*sizeof(val16)),&val16,sizeof(val16));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy  out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val16)); i++) {
			copy(&val16,bytes+(i*sizeof(val16)),sizeof(val16));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val16)); i++) {
			copy8(&val16,bytes+(i*sizeof(val16)),sizeof(val16));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);



	stdoutput.printf("uint32_t\n");
	stdoutput.printf("	copy  in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	uint32_t	val32=0xFFFFFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val32)); i++) {
			copy(bytes+(i*sizeof(val32)),&val32,sizeof(val32));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy  in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	val32=0xFFFFFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val32)); i++) {
			copy8(bytes+(i*sizeof(val32)),&val32,sizeof(val32));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy  out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val32)); i++) {
			copy(&val32,bytes+(i*sizeof(val32)),sizeof(val32));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val32)); i++) {
			copy8(&val32,bytes+(i*sizeof(val32)),sizeof(val32));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);



	stdoutput.printf("uint64_t\n");
	stdoutput.printf("	copy  in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	uint64_t	val64=0xFFFFFFFFFFFFFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val64)); i++) {
			copy(bytes+(i*sizeof(val64)),&val64,sizeof(val64));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 in : ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	val64=0xFFFFFFFFFFFFFFFF;
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val64)); i++) {
			copy8(bytes+(i*sizeof(val64)),&val64,sizeof(val64));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy  out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val64)); i++) {
			copy(&val64,bytes+(i*sizeof(val64)),sizeof(val64));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);

	stdoutput.printf("	copy8 out: ");
	stdoutput.flush();
	start.getSystemDateAndTime();
	for (uint16_t j=0; j<ITERATIONS; j++) {
		for (uint64_t i=0; i<(BUFFERSIZE/sizeof(val64)); i++) {
			copy8(&val64,bytes+(i*sizeof(val64)),sizeof(val64));
		}
	}
	end.getSystemDateAndTime();
	displayTime(&start,&end);
}
