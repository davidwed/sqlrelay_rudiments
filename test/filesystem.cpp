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
				(uint32_t)fs.getType());
	printf("	block size:			%lld\n",
				fs.getBlockSize());
	printf("	optimum tranfer block size:	%lld\n",
				fs.getOptimumTransferBlockSize());
	printf("	total blocks:			%lld\n",
				fs.getTotalBlocks());
	printf("	free blocks:			%lld\n",
				fs.getFreeBlocks());
	printf("	available blocks:		%lld\n",
				fs.getAvailableBlocks());
	printf("	total nodes:			%lld\n",
				fs.getTotalFileNodes());
	printf("	free nodes:			%lld\n",
				fs.getFreeFileNodes());
	printf("	available nodes:		%lld\n",
				fs.getAvailableFileNodes());
	printf("	filesystem id:			%lld\n",
				fs.getFileSystemId());
	printf("	maximum file name length:	%lld\n",
				fs.getMaximumFileNameLength());
	printf("	owner:				%d\n",
				fs.getOwner());
	printf("	sync writes:			%lld\n",
				fs.getSyncWrites());
	printf("	async writes:			%lld\n",
				fs.getAsyncWrites());
	printf("	fs type name:			%s\n",
				fs.getTypeName());
	printf("	mount point:			%s\n",
				fs.getMountPoint());
	printf("	sync reads:			%lld\n",
				fs.getSyncReads());
	printf("	async reads:			%lld\n",
				fs.getAsyncReads());
	printf("	device name:			%s\n",
				fs.getDeviceName());
	printf("	fs specific string:		%s\n",
				fs.getFilesystemSpecificString());
}
