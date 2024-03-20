#include "libTinyFS.h"

// THis file prints if buff is called!
int main(int argc, char *argv[]) {
    fileDescriptor t;
    // test string to get readByte from
    char buff[1] = " ";

    // test string greater than 256
    char test[400] = "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest";
    
    // make a file called file
    tfs_mkfs("file", BLOCKSIZE * 5, "haha");
    
    // mount it
    printf("%d\n",tfs_mount("file", "haha"));
    return 1;
    // open a fake file called file2
    t = tfs_openFile("file2");
    // write the long string to it
    tfs_writeFile(t,test, 400);

    // remove the long string
    tfs_deleteFile(t);

    // same process as above till like 28
    tfs_mount("file", "bad");
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
    //close the file.
    tfs_closeFile(t);
    return 0;
}