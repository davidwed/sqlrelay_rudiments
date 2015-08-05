#include <rudiments/file.h>
#include <rudiments/permissions.h>

int main(int argc, const char **argv) {

	// open/create the file
	file	f;
	f.open("testfile",O_WRONLY|O_CREAT,
				permissions::evalPermString("rw-rw-rw-"));


	// write a bool
	bool	b=true;
	f.write(b);


	// write various characters
	char	c='a';
	f.write(c);

	unsigned char	uc='a';
	f.write(uc);


	// write various integers
	uint16_t	ui16=16;
	f.write(ui16);

	uint32_t	ui32=32;
	f.write(ui32);

	uint64_t	ui64=64;
	f.write(ui64);

	int16_t		i16=-16;
	f.write(i16);

	int32_t		i32=-32;
	f.write(i32);

	int64_t		i64=-64;
	f.write(i64);


	// write various floats
	float		fl=1.234;
	f.write(fl);

	double		db=1.234;
	f.write(db);


	// write some text
	const char	*text="hello there";
	f.write(text);

	// write the first 5 bytes of the same text
	f.write(text,5);


	// write some binary data
	unsigned char	binary[]={1,2,3,4,5,6,7,8,9,0};
	f.write(binary,sizeof(binary));


	// write arbitary binary data
	uint64_t	data[]={12345,67890,12345,67890};
	f.write((void *)data,sizeof(data));
}
