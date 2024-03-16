#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "libDisk.h"
#include "tinyFS.h"
#include "TinyFS_errno.h"

int openDisk(char *filename, int nBytes) {
    // validate disk size
    if (nBytes < 0 || (1 < nBytes && nBytes < BLOCKSIZE)){
        return EDISKSIZE;
    }

    // validate file name
    if (filename == NULL) {
      return EFILENAME;
    }    

    int fd;
    int clip = nBytes % BLOCKSIZE;
    nBytes -= clip;

    if (nBytes == 0) {
        if ((fd = open(filename, O_RDWR)) == -1) {
            return EFILENOTEXIST;
        }
    }
    else {
        if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR |
                                S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1) {
            return EFILECREATE;
        }
    }

   return fd;
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