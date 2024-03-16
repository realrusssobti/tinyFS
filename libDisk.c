#include <stdio.h>
#include <stdlib.h>
#include "libDisk.h"
#include "tinyFS.h"
// wondering where we need to define this, I'll keep it here for now
// because its needed in tinyFS, but also here.

typedef struct disk {
    FILE *fileptr;
    char *buffer;
} disk;

// I'm thinking of storing these in a linked list? what do we think?

// Is there a max number of disks to look at?
int openDisk(char *filename, int nBytes) {
    int clip = nBytes % BLOCKSIZE;

    if (clip != 0) {
        nBytes = nBytes - clip;
    } 
    if (nBytes == 0) {
        exit(1);
        // return an existing disk with the filename value.
    }
    if (nBytes < BLOCKSIZE) {
        exit(1);
        // RETURN FAILURE
    }

    // Loop through the disk list, check if the filename is already used,
    // if so, then open it up

    // Make a new disk here
    //block *newblock = malloc();
    
    // open the file with read and write permissions.
    // maybe there is another way to do this without fopen, but lmk if theres something else
    //FILE *fp = *fopen(filename, "r+");



    
    // loop through existing disks, and check filenames.
    // if its the same, replace it at that index.
    // if there are none of the same, make a new disk.
    return -1;

}
int closeDisk(int disk) {
    // free up the index given by the value.
    return -1;
}

int readBlock(int disk, int bNum, void *block) {
    // read byte offsets, 
    return -1;
}

int writeBlock(int disk, int bNum, void *block){
    return -1;
}