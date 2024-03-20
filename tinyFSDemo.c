#include "libTinyFS.h"
#include <stdio.h>


// THis file prints if buff is called!
int main(int argc, char *argv[]) {
    fileDescriptor t;
    // test string to get readByte from
    char buff[1] = " ";

    // test string greater than 256
    char test[400] = "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest";

    // make a file called file
    tfs_mkfs("file", BLOCKSIZE * 10);

    // mount it
    tfs_mount("file");

    // open a fake file called file2
    t = tfs_openFile("file2");
    // write the long string to it
    tfs_writeFile(t,test, 400);

    // remove the long string
    tfs_deleteFile(t);

    // same process as above till like 28
    tfs_mount("file");
    t = tfs_openFile("file2");
    tfs_writeFile(t, "this is a phrase", 17);

    // read the first byte of the short string
    tfs_readByte(t,buff);
    printf("%s\n", buff);

    // read the second byte
    tfs_readByte(t,buff);
    printf("%s\n", buff);

    // move to the 7th byte
    tfs_seek(t, 7);

    // read the 7th
    tfs_readByte(t,buff);
    printf("%s\n", buff);

    //read the 8th
    tfs_readByte(t,buff);
    printf("%s\n", buff);

	//read the 10th
	tfs_seek(t,10);
	tfs_readByte(t,buff);
	printf("%s\n", buff);
    //close the file.
	tfs_closeFile(t);
	// create a new file
	t = tfs_openFile("file3");
	// write to it
	tfs_writeFile(t, "this is a phrase", 17);
	// Make it read only
	tfs_makeRO("file3");
	// try to write to it
	tfs_writeByte(t, 'a');
	// make it read write
	tfs_makeRW("file3");
	// write to it
	tfs_seek(t, 0);
	tfs_writeByte(t, 'a');
	// seek to the 0th byte
	tfs_seek(t, 0);
	// read the first byte
	tfs_readByte(t,buff);
	// read file info
	tfs_readFileInfo(t);

	// close it
	printf("Renaming file3 to file69\n");
	tfs_rename(t, "file69");
	// demo the readdir
	tfs_readdir();
	tfs_displayFragments();
	tfs_closeFile(t);

	tfs_unmount();



    return 0;
}