#include "tinyFS.h"
#include "libTinyFS.h"
#include "libDisk.h"
#include "linkedList.h"
#include "TinyFS_errno.h"
#include <unistd.h>

// store the current disk as a global variable
int mounted = -1;
// filetable for files that are open
list_t *fileTable = NULL;
// the max index of the filetable.
int fdmax = 1;

int tfs_mkfs(char *filename, int nBytes){
    int diskNum = openDisk(filename, nBytes), i;
    if (diskNum < 0) {
        exit(1);
        // exit with error
    }

    char buffer[256];
    for (i = 0; i < 256; i++) {
        buffer[i] = 0;
    }
    buffer[0] = '1';
    buffer[1] = 'D';
    // for the free block list
    buffer[2] = 1;

    int errno =  writeBlock(diskNum, 0, buffer);
    // if error, do something.

    buffer[0] = '4';
    int diskSize = lseek(diskNum, 0, SEEK_END);
    int i = 1;
    // write in all of the empty blocks
    for (i; i < diskSize;i++) {
        // Set the link for the next free block
        if (i != diskSize - 1) {
            buffer[2] = i + 1;
        }
        writeBlock(diskNum, i, buffer);
    }
    closeDisk(diskNum);
    return 1;
}


int tfs_mount(char *diskname){
    // Checks if there is something mounted
    if (mounted != -1) {
        return EMOUNTED;
    }
    int diskNum = openDisk(diskname, 0);
    if (diskNum < 0) {
        exit(1);
        // exit with error
    }

    char * buffer;
    int diskSize = lseek(diskNum, 0, SEEK_END)  / 256;
    int i;
    // check that all the blocks have the correct checksum: 0x44
    for (i = 1; i < diskSize;i++) {
        readBlock(diskNum, i, buffer);
        if (buffer[1] != 'D') {
            return EINVALIDTFS;
        }
    }
    // set the currently mounted disk to the disk
    mounted = diskNum;
    return 1;
}

int tfs_unmount(void){
    // checks if there is something mounted
    if (mounted == -1) {
        return EUNMOUNTED;
    }
    // wipes the file table
    cleanListFree(fileTable);
    // closes the disk opened by mount
    closeDisk(mounted);
    return 1;
}

fileDescriptor tfs_openFile(char *name){
    int location;
    char * superblock, *newInode;
    // if the filetable hasn't been initialized, do it
    if (fileTable == NULL) {
        fileTable = malloc(sizeof(list_t));
    }

    resNode *newNode = NULL;
    // check if an inode exists for a file. If it doesn't make one. 
    if ((location = checkFileExists(name)) == 0) {
        // read the next free block off the superblock
        readBlock(mounted, 0, superblock);
        location = superblock[2];
        // read that block to set the superblock's new free list value
        readBlock(mounted, location, newInode);
        superblock[2] = newInode[2];
        
        // set it that there is no data for this inode
        newInode[2] = -1;

        // write changes back to the file
        writeBlock(mounted,0,superblock);
        writeBlock(mounted,location,newInode);
    }
    // setup a new node to add to the fileTable
    newNode->fd = fdmax++;
    newNode->inodeIndex = location;
    strncpy(newNode->name, name, FILENAME_LEN);

    addtoList(fileTable, newNode);
    return newNode->fd;
}

int tfs_closeFile(fileDescriptor FD){
    resNode *data;
    // remove from the filetable
    data = removeVal(FD, fileTable);
    // free up that space
    free(data);
    return 0;
}

int tfs_writeFile(fileDescriptor FD,char *buffer, int size){
    return 0;
}

int tfs_deleteFile(fileDescriptor FD){
    return 0;
}

int tfs_readByte(fileDescriptor FD, char *buffer){
    return 0; 
}

int tfs_seek(fileDescriptor FD, int offset){
    return 0;
}

// returns the block where the file inode is (if it exists)
int checkFileExists(char * name) {
    int i;
    int diskSize = lseek(mounted, 0, SEEK_END) / 256;
    char * buffer, *check;
    for (i = 1; i < diskSize; i++) {
        readBlock(mounted, i, buffer);
        if (buffer[0] == 2) {
            strncpy(check, buffer[4], 8);
            if (strcmp(name, check) == 0) return i;
        }
    }
    return -1;
}