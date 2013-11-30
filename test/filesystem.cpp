// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/filesystem.h>
#include <rudiments/stdio.h>

#ifdef _WIN32
const char	*root="C:\\";
#else
const char	*root="/";
#endif

int main(int argc, const char **argv) {

	filesystem	fs;
	fs.initialize(root);

	stdoutput.printf("%s fileststem stats:\n",root);
	stdoutput.printf("	type:				0x%08x\n",
				(uint32_t)fs.getType());
	stdoutput.printf("	block size:			%lld\n",
				fs.getBlockSize());
	stdoutput.printf("	optimum tranfer block size:	%lld\n",
				fs.getOptimumTransferBlockSize());
	stdoutput.printf("	total blocks:			%lld\n",
				fs.getTotalBlocks());
	stdoutput.printf("	free blocks:			%lld\n",
				fs.getFreeBlocks());
	stdoutput.printf("	available blocks:		%lld\n",
				fs.getAvailableBlocks());
	stdoutput.printf("	total nodes:			%lld\n",
				fs.getTotalFileNodes());
	stdoutput.printf("	free nodes:			%lld\n",
				fs.getFreeFileNodes());
	stdoutput.printf("	available nodes:		%lld\n",
				fs.getAvailableFileNodes());
	stdoutput.printf("	filesystem id:			%lld\n",
				fs.getFileSystemId());
	stdoutput.printf("	maximum file name length:	%lld\n",
				fs.getMaximumFileNameLength());
	stdoutput.printf("	owner:				%d\n",
				fs.getOwner());
	stdoutput.printf("	sync writes:			%lld\n",
				fs.getSyncWrites());
	stdoutput.printf("	async writes:			%lld\n",
				fs.getAsyncWrites());
	stdoutput.printf("	fs type name:			%s\n",
				fs.getTypeName());
	stdoutput.printf("	mount point:			%s\n",
				fs.getMountPoint());
	stdoutput.printf("	sync reads:			%lld\n",
				fs.getSyncReads());
	stdoutput.printf("	async reads:			%lld\n",
				fs.getAsyncReads());
	stdoutput.printf("	device name:			%s\n",
				fs.getDeviceName());
	stdoutput.printf("	fs specific string:		%s\n",
				fs.getFilesystemSpecificString());
}
