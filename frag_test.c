#include <stdio.h>
#include <string.h>
#include "libTinyFS.h"

void generateFragmentedDisk() {
	// Create a file system
	tfs_mkfs("fragmentedDisk", BLOCKSIZE * 25);

	// Mount the file system
	tfs_mount("fragmentedDisk");

	fileDescriptor fds[10];

	// Open and write to multiple files
	for (int i = 0; i < 10; i++) {
		char filename[10];
		sprintf(filename, "file%d", i);
		printf("Creating file %s\n", filename);
		fds[i] = tfs_openFile(filename);
		char data[BLOCKSIZE];
		sprintf(data, "This is file %d", i);
		tfs_writeFile(fds[i], data, strlen(data));
	}

	// read out all the files first
	for (int i = 0; i < 10; i++) {
		char filename[10];
		sprintf(filename, "file%d", i);

	}
//	tfs_displayFragments();

	// delete file 3
	tfs_deleteFile(fds[3]);

	// Display the fragments
	tfs_displayFragments();

	// Defragment the disk
	tfs_defrag();

	// Display the fragments
	tfs_displayFragments();

	// Unmount the file system
	tfs_unmount();
}

int main(int argc, char *argv[]) {
	generateFragmentedDisk();
	return 0;
}