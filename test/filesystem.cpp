// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/filesystem.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/userentry.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char	*filename=
#ifdef _WIN32
	"filesystem.exe";
#else
	"filesystem";
#endif

void print(filesystem *fs) {
	stdoutput.printf("fileststem stats:\n");
	stdoutput.printf("	type:				0x%08x\n",
				(uint32_t)fs->getType());
	stdoutput.printf("	block size:			%lld\n",
				fs->getBlockSize());
	stdoutput.printf("	optimum tranfer block size:	%lld\n",
				fs->getOptimumTransferBlockSize());
	stdoutput.printf("	total blocks:			%lld\n",
				fs->getTotalBlocks());
	stdoutput.printf("	free blocks:			%lld\n",
				fs->getFreeBlocks());
	stdoutput.printf("	available blocks:		%lld\n",
				fs->getAvailableBlocks());
	stdoutput.printf("	reserved blocks:		%lld\n",
				fs->getReservedBlocks());
	stdoutput.printf("	total nodes:			%lld\n",
				fs->getTotalFileNodes());
	stdoutput.printf("	free nodes:			%lld\n",
				fs->getFreeFileNodes());
	stdoutput.printf("	available nodes:		%lld\n",
				fs->getAvailableFileNodes());
	stdoutput.printf("	reserved nodes:			%lld\n",
				fs->getReservedFileNodes());
	stdoutput.printf("	filesystem id:			%lld\n",
				fs->getFileSystemId());
	stdoutput.printf("	maximum file name length:	%lld\n",
				fs->getMaximumFileNameLength());
	stdoutput.printf("	owner:				%d\n",
				fs->getOwner());
	stdoutput.printf("	sync writes:			%lld\n",
				fs->getSyncWrites());
	stdoutput.printf("	async writes:			%lld\n",
				fs->getAsyncWrites());
	stdoutput.printf("	fs type name:			%s\n",
				fs->getTypeName());
	stdoutput.printf("	mount point:			%s\n",
				fs->getMountPoint());
	stdoutput.printf("	sync reads:			%lld\n",
				fs->getSyncReads());
	stdoutput.printf("	async reads:			%lld\n",
				fs->getAsyncReads());
	stdoutput.printf("	device name:			%s\n",
				fs->getDeviceName());
	stdoutput.printf("	fs specific string:		%s\n\n",
				fs->getFilesystemSpecificString());
}

int main(int argc, const char **argv) {

	header("filesystem");

	filesystem	fs;
	for (uint16_t i=0; i<2; i++) {

		if (i==0) {
			stdoutput.printf("uninitialized...\n");
		} else if (i==1) {
			stdoutput.printf("negative file descriptor...\n");
			fs.initialize(-1);
		}

		//print(&fs);

		test("type",!fs.getType());
		test("block size",!fs.getBlockSize());
		test("optimum transfer block size",
				!fs.getOptimumTransferBlockSize());
		test("total blocks",!fs.getTotalBlocks());
		test("free blocks",!fs.getFreeBlocks());
		test("available blocks",!fs.getAvailableBlocks());
		test("reserved blocks",!fs.getReservedBlocks());
		test("total file nodes",!fs.getTotalFileNodes());
		test("free file nodes",!fs.getFreeFileNodes());
		test("available file nodes",!fs.getAvailableFileNodes());
		test("resreved file nodes",!fs.getReservedFileNodes());
		test("file system id",!fs.getFileSystemId());
		test("maximum file name length",!fs.getMaximumFileNameLength());
		test("owner",!fs.getOwner());
		test("sync writes",!fs.getSyncWrites());
		test("async writes",!fs.getAsyncWrites());
		test("type name",!fs.getTypeName());
		test("mount point",!fs.getMountPoint());
		test("sync reads",!fs.getSyncReads());
		test("async reads",!fs.getAsyncReads());
		test("device name",!fs.getDeviceName());
		test("filesystem specific string",
				!fs.getFilesystemSpecificString());
		stdoutput.printf("\n");
	}

	stdoutput.printf("from file name...\n");
	filesystem	fs1;
	fs1.initialize(filename);
	//print(&fs1);
	test("type",fs1.getType());
	test("block size",fs1.getBlockSize());
	test("optimum transfer block size",fs1.getOptimumTransferBlockSize());
	test("total blocks",fs1.getTotalBlocks());
	test("free blocks",fs1.getFreeBlocks());

	// Windows doesn't really have "nodes"
	#ifndef _WIN32
		test("total file nodes",fs1.getTotalFileNodes());
		test("free file nodes",fs1.getFreeFileNodes());
	#endif

	test("file system id",fs1.getFileSystemId());
	test("maximum file name length",fs1.getMaximumFileNameLength());

	// Windows doesn't have filesystem owners
	#ifndef _WIN32
		char	*username=userentry::getName(fs1.getOwner());
		test("owner",!charstring::isNullOrEmpty(username));
		delete[] username;
	#endif

	test("type name",fs1.getTypeName());

	// believe it or not, these aren't known on most systems...
	//test("mount point",fs1.getMountPoint());
	//test("device name",fs1.getDeviceName());

	stdoutput.printf("\n");

	for (uint16_t i=0; i<2; i++) {

		filesystem	fs2;
		stdoutput.printf("from file name vs. ");
		if (i==0) {
			stdoutput.printf("from file descriptor...\n");
			file	f;
			f.open(filename,O_RDONLY);
			fs2.initialize(f.getFileDescriptor());
		} else {
			stdoutput.printf("from NULL...\n");
			fs2.initialize((const char *)NULL);
		}
		//print(&fs2);

		test("type",
			fs1.getType()==fs2.getType());
		test("block size",
			fs1.getBlockSize()==fs2.getBlockSize());
		test("optimum transfer block size",
			fs1.getOptimumTransferBlockSize()==
			fs2.getOptimumTransferBlockSize());
		test("total blocks",
			fs1.getTotalBlocks()==fs2.getTotalBlocks());
		test("free blocks",
			fs1.getFreeBlocks()==fs2.getFreeBlocks());
		test("available blocks",
			fs1.getAvailableBlocks()==fs2.getAvailableBlocks());
		test("reserved blocks",
			fs1.getReservedBlocks()==fs2.getReservedBlocks());
		test("total file nodes",
			fs1.getTotalFileNodes()==fs2.getTotalFileNodes());
		test("free file nodes",
			fs1.getFreeFileNodes()==fs2.getFreeFileNodes());
		test("available file nodes",
			fs1.getAvailableFileNodes()==
			fs2.getAvailableFileNodes());
		test("reserved file nodes",
			fs1.getReservedFileNodes()==
			fs2.getReservedFileNodes());
		test("file system id",
			fs1.getFileSystemId()==fs2.getFileSystemId());
		test("maximum file name length",
			fs1.getMaximumFileNameLength()==
			fs2.getMaximumFileNameLength());
		test("owner",
			fs1.getOwner()==fs2.getOwner());
		test("sync writes",
			fs1.getSyncWrites()==fs2.getSyncWrites());
		test("async writes",
			fs1.getAsyncWrites()==fs2.getAsyncWrites());
		test("type name",
			!charstring::compare(fs1.getTypeName(),
						fs2.getTypeName()));
		test("mount point",
			!charstring::compare(fs1.getMountPoint(),
						fs2.getMountPoint()));
		test("sync reads",
			fs1.getSyncReads()==fs2.getSyncReads());
		test("async reads",
			fs1.getAsyncReads()==fs2.getAsyncReads());
		test("device name",
			!charstring::compare(fs1.getDeviceName(),
						fs2.getDeviceName()));
		test("filesystem specific string",
			!charstring::compare(
				fs1.getFilesystemSpecificString(),
				fs2.getFilesystemSpecificString()));
		stdoutput.printf("\n");
	}
}
