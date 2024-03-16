#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    // initialize the file with 0's
    int i;
    for (i = 0; i < nBytes; i++) {
         if (write(fd, "\0", 1) == -1) {
            return EDISKWRITE; 
        }
    }

   return fd;
}

int closeDisk(int disk) {

    // validate file descriptor
    if (disk < 0) {
        return EDISKNUMBER;
    }

    if (close(disk) == -1) {
        return EDISKCLOSE;
    }

   return 0;
}

int readBlock(int disk, int bNum, void *block) {
    // validate file descriptor
    if (disk < 0) {
      return EDISKNUMBER;
    }

    // validate block number
    if (bNum < 0){
        return EBLOCKNUMBER;
    }

    // validate block memory
    if (block == NULL){
        return EBLOCKNULL;
    }

    int diskSize = -1;

    // get the disk size
    if ((diskSize = lseek(disk, 0, SEEK_END)) == -1) {
      return EDISKSEEK;
    }

    // validate bNum is within the disk size
    if (((bNum + 1) * BLOCKSIZE) > diskSize) {
        return EBLOCKNUMBER;
    }

    // set seek position
    if (lseek(disk, bNum * BLOCKSIZE, SEEK_SET) != (bNum * BLOCKSIZE)) {
        return EDISKSEEK;
    }

    // read a block
    if (read(disk, block, BLOCKSIZE) == -1) {
        return EBLOCKREAD;
    }

    return 0;
}

int writeBlock(int disk, int bNum, void *block) {
    // validate file descriptor
    if (disk < 0) {
      return EDISKNUMBER;
    }

    // validate block number
    if (bNum < 0){
        return EBLOCKNUMBER;
    }

    // validate block memory
    if (block == NULL){
        return EBLOCKNULL;
    }

    int diskSize = -1;

    // get the disk size
    if ((diskSize = lseek(disk, 0, SEEK_END)) == -1) {
      return EDISKSEEK;
    }

    // validate bNum is within the disk size
    if (((bNum + 1) * BLOCKSIZE) > diskSize) {
        return EBLOCKNUMBER;
    }

    // set seek position
    if (lseek(disk, bNum * BLOCKSIZE, SEEK_SET) != (bNum * BLOCKSIZE)) {
        return EDISKSEEK;
    }

    // write a block
    if (write(disk, block, BLOCKSIZE) == -1) {
        return EBLOCKWRITE;
    }

    return 0;
}
