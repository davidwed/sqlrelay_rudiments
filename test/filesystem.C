// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/filesystem.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	filesystem	fs;
	fs.initialize("/");

	printf("root fileststem stats:\n");
	printf("	type:				0x%08x\n",
				fs.getType());
	printf("	block size:			%ld\n",
				fs.getBlockSize());
	printf("	optimum tranfer block size:	%ld\n",
				fs.getOptimumTransferBlockSize());
	printf("	total blocks:			%ld\n",
				fs.getTotalBlocks());
	printf("	free blocks:			%ld\n",
				fs.getFreeBlocks());
	printf("	available blocks:		%ld\n",
				fs.getAvailableBlocks());
	printf("	total nodes:			%ld\n",
				fs.getTotalFileNodes());
	printf("	free nodes:			%ld\n",
				fs.getFreeFileNodes());
	printf("	available nodes:		%ld\n",
				fs.getAvailableFileNodes());
	printf("	filesystem id:			%ld\n",
				fs.getFileSystemId());
	printf("	maximum file name length:	%ld\n",
				fs.getMaximumFileNameLength());
	printf("	owner:				%ld\n",
				fs.getOwner());
	printf("	sync writes:			%ld\n",
				fs.getSyncWrites());
	printf("	async writes:			%ld\n",
				fs.getAsyncWrites());
	printf("	fs type name:			%s\n",
				fs.getTypeName());
	printf("	mount point:			%s\n",
				fs.getMountPoint());
	printf("	sync reads:			%ld\n",
				fs.getSyncReads());
	printf("	async reads:			%ld\n",
				fs.getAsyncReads());
	printf("	device name:			%s\n",
				fs.getDeviceName());
	printf("	fs specific string:		%s\n",
				fs.getFilesystemSpecificString());
}
