#include "libTinyFS.h"
#include <stdio.h>


// THis file prints if buff is called!
	int main(int argc, char *argv[]) {
		fileDescriptor fd1, fd2, fd3;
		char buff[1] = " ";
		char test[400] = "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest";

		// Create a file system
		tfs_mkfs("file", BLOCKSIZE * 20);

		// Mount the file system
		tfs_mount("file");

		// Open three files
		fd1 = tfs_openFile("file1");
		fd2 = tfs_openFile("file2");
		fd3 = tfs_openFile("file3");

		// Write to the files
		tfs_writeFile(fd1, test, 400);
		tfs_writeFile(fd2, "this is a phrase", 17);
		tfs_writeFile(fd3, "another phrase", 14);

		// Delete the second file to create a hole in the disk
		tfs_deleteFile(fd2);

		// Display the fragments before defragmentation
		printf("Before defragmentation:\n");
		tfs_displayFragments();

		// Defragment the disk
		tfs_defrag();

		// Display the fragments after defragmentation
		printf("After defragmentation:\n");
		tfs_displayFragments();

		// Close the files
		tfs_closeFile(fd1);
		tfs_closeFile(fd3);

		return 0;
	}
