#include <rudiments/file.h>

int main(int argc, const char **argv) {

	// open the file
	file	f;
	f.open("testfile",O_RDONLY);


	// read a bool
	bool	b;
	f.read(&b);


	// read various characters
	char	c;
	f.read(&c);

	unsigned char	uc;
	f.read(&uc);


	// read various integers
	uint16_t	ui16;
	f.read(&ui16);

	uint32_t	ui32;
	f.read(&ui32);

	uint64_t	ui64;
	f.read(&ui64);

	int16_t		i16;
	f.read(&i16);

	int32_t		i32;
	f.read(&i32);

	int64_t		i64;
	f.read(&i64);


	// read various floats
	float		fl;
	f.read(&fl);

	double		db;
	f.read(&db);
}
