// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/filesysteminlines.h>
#endif

#ifdef HAVE_LINUX_STATFS
	char *filesystem::getFsTypeName(long fstype) {
		if (fstype==0xADF5) {
			return "adfs";
		} else if (fstype==0xADFF) {
			return "affs";
		} else if (fstype==0x1BADFACE) {
			return "bfs";
		} else if (fstype==0x28cd3d45) {
			return "cramfs";
		} else if (fstype==0x00414A53) {
			return "efs";
		} else if (fstype==0x137D) {
			return "ext";
		} else if (fstype==0xEF51) {
			return "ext2 (old)";
		} else if (fstype==0xEF53) {
			return "ext2";
		// hfs???
		} else if (fstype==0xF995E849) {
			return "hpfs";
		} else if (fstype==0x9660) {
			return "iso";
		} else if (fstype==0x137F) {
			return "minix (original)";
		} else if (fstype==0x138F) {
			return "minix (30 char)";
		} else if (fstype==0x2468) {
			return "minix2 (original)";
		} else if (fstype==0x2478) {
			return "minix2 (30 char)";
		} else if (fstype==0x4d44) {
			return "msdos";
		} else if (fstype==0x564c) {
			return "ncp";
		} else if (fstype==0x6969) {
			return "nfs";
		} else if (fstype==0x9fa1) {
			return "openprom";
		} else if (fstype==0x9fa0) {
			return "proc";
		} else if (fstype==0x002f) {
			return "qnx4";
		} else if (fstype==0x52654973) {
			return "reiserfs";
		} else if (fstype==0x7275) {
			return "romfs";
		} else if (fstype==0x517B) {
			return "smb";
		} else if (fstype==0x012FF7B4) {
			return "xenix";
		} else if (fstype==0x012FF7B5) {
			return "sysv4";
		} else if (fstype==0x012FF7B6) {
			return "sysv2";
		} else if (fstype==0x012FF7B7) {
			return "coherent";
		} else if (fstype==0x00011954) {
			return "ufs";
		} else if (fstype==0x58465342) {
			return "xfs";
		} else if (fstype==0x012FD16D) {
			return "xiafs";
		} else if (fstype==0x9fa2) {
			return "usbdevfs";
		}
		return NULL;
	}
#endif

