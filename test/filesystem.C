// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/filesystem.h>
#include <stdio.h>

int main(int argv, const char **argc) {

	filesystem	fs;
	fs.initialize("/");

	printf("root fileststem stats:\n");
	printf("	type:				0x%08x\n",
				fs.getType());
	printf("	block size:			%d\n",
				fs.getBlockSize());
	printf("	optimum tranfer block size:	%d\n",
				fs.getOptimumTransferBlockSize());
	printf("	total blocks:			%d\n",
				fs.getTotalBlocks());
	printf("	free blocks:			%d\n",
				fs.getFreeBlocks());
	printf("	available blocks:		%d\n",
				fs.getAvailableBlocks());
	printf("	total nodes:			%d\n",
				fs.getTotalFileNodes());
	printf("	free nodes:			%d\n",
				fs.getFreeFileNodes());
	printf("	available nodes:		%d\n",
				fs.getAvailableFileNodes());
	printf("	filesystem id:			%d\n",
				fs.getFileSystemId());
	printf("	maximum file name length:	%d\n",
				fs.getMaximumFileNameLength());
	printf("	owner:				%d\n",
				fs.getOwner());
	printf("	sync writes:			%d\n",
				fs.getSyncWrites());
	printf("	async writes:			%d\n",
				fs.getAsyncWrites());
	printf("	fs type name:			%s\n",
				fs.getTypeName());
	printf("	mount point:			%s\n",
				fs.getMountPoint());
	printf("	sync reads:			%d\n",
				fs.getSyncReads());
	printf("	async reads:			%d\n",
				fs.getAsyncReads());
	printf("	device name:			%s\n",
				fs.getDeviceName());
	printf("	fs specific string:		%s\n",
				fs.getFilesystemSpecificString());
}
