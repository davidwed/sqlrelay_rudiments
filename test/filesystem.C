// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/filesystem.h>
#include <stdio.h>

int main(int argv, const char **argc) {

	filesystem	fs;
	fs.initialize("/");

	printf("root fileststem stats:\n");
	printf("	type:				%d\n",
				fs.getType());
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
	printf("	filesystem id:			%d\n",
				fs.getFileSystemId());
	printf("	maximum file name length:	%d\n",
				fs.getMaximumFileNameLength());
}
