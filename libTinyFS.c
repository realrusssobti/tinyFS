#include "tinyFS.h"
#include "libTinyFS.h"
#include "libDisk.h"
#include "linkedList.h"
#include "TinyFS_errno.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/*
IMPORTANT NOTES:
Storage of blocks:
ALL LINKS ARE STORED ON block[2]
if you want to add links to directory, would reccomend using block[3]

USE THE FOLLOWING TO CHECK IF THE OUTPUT IS CORRECT. HEXDUMP
xxd <disk filename>



*/

// store the current disk as a global variable
int mounted = -1;
// filetable for files that are open
list_t *fileTable = NULL;
// the max index of the filetable.
int fdmax = 1;

int tfs_mkfs(char *filename, int nBytes){
    // open the disk for working
    int diskNum = openDisk(filename, nBytes), i;
    if (diskNum < 0) {
        return EDISKFAILED;
    }

    // make a buffer for adding things
    uint8_t *buffer = calloc(sizeof(char), 256);

    // set super block type, magic number, x44 = D
    buffer[0] = 0;
    buffer[1] = 'D';
    // for the free block list
    buffer[2] = 1;

    //write the contents to the super block.
    writeBlock(diskNum, 0, buffer);
    
    // set the type for the empty blocks
    buffer[0] = 4;
    int diskSize = lseek(diskNum, 0, SEEK_END) / 256;
    // write in all of the empty blocks
    for (i = 1; i < diskSize;i++) {
        // Set the link for the next free block
        if (i != (diskSize - 1)) {
            buffer[2] = i + 1;
        } else {
            // if its the last block, dont add a link, go to 0
            buffer[2] = 0;
        }
        // write the empty block
        writeBlock(diskNum, i, buffer);
    }
    free(buffer);
    // open the disk if you want to use it later.
    closeDisk(diskNum);
    return 1;
}

int tfs_mount(char *diskname){
    // Checks if there is something mounted
    if (mounted != -1) {
        return EMOUNTED;
    }
    // opens up disk. To be closed by unount
    int diskNum = openDisk(diskname, 0);
    if (diskNum < 0) {
        return EDISKFAILED;
        // exit with error
    }

    // makes a buffer to do stuff with.
    uint8_t buffer[256];
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
    // sets it so that theres nothing mounted.
    mounted = -1;
    // closes the disk opened by mount
    closeDisk(mounted);
    return 1;
}

fileDescriptor tfs_openFile(char *name){
    // location of the inode
    int location;
    // blocks for superbloc and inode
    uint8_t superblock[BLOCKSIZE], newInode[BLOCKSIZE];
    // if the filetable hasn't been initialized, do it
    if (fileTable == NULL) {
        fileTable = makeList();
    }

    // malloc a new node 
    resNode *newNode = malloc(sizeof(resNode));
    // check if an inode exists for a file. If it doesn't make one. 
    if ((location = checkFileExists(name)) == 0) {
        // read the next free block off the superblock
        readBlock(mounted, 0, superblock);
        // this is the new inode
        location = superblock[2];
        // read that block to set the superblock's new free list value
        readBlock(mounted, location, newInode);
        superblock[2] = newInode[2];
        
        // write changes back to the file
        writeBlock(mounted,0,superblock);
    } else {
        // if it exists, use it and proceed.
        readBlock(mounted, location, newInode);
    }
    
    // set it that there is no data for this inode
    newInode[2] = 0;
    // set the type to be an inode
    newInode[0] = 2;
    // set the first 8 data bytes to be a name
    strncpy(newInode + 4, name, 8);
    // write back to disk
    writeBlock(mounted,location,newInode);
    
    // setup a new node to add to the fileTable
    newNode->fd = fdmax++;
    newNode->fp = 0;
    newNode->inodeIndex = location;
    strncpy(newNode->name, name, FILENAME_LEN);

    // add to the list using linkedlist implementation
    addtoList(fileTable, newNode);
    return newNode->fd;

}

int tfs_closeFile(fileDescriptor FD){
    resNode *data;
    // remove from the filetable
    removeVal(FD, fileTable);
    // the online one just got rid of everything when closeFile was called... 
    // We can maybe do better? I did it like the online one...
    cleanListFree(fileTable);
    fdmax = 1;
    fileTable = NULL;
    tfs_unmount();
    return 0;
}


int tfs_writeFile(fileDescriptor FD,char *buffer, int size){
    // create buffers for what to write, the super block, and extra blocks.
    uint8_t * message;
    uint8_t block[BLOCKSIZE] = "";
    uint8_t super[BLOCKSIZE] = "";

    int blockNum, inode, block_size;
    resNode *n;
    // checks if the file is in the FD.
    n = searchListFD(fileTable, FD);
    inode = n->fd;
    if (n == NULL) {
        return -1;
        //error
    }
    // read the inode to block
    readBlock(mounted, inode, block);
    // read the superblock to super
    readBlock(mounted, 0, super);
    // pick up the first free block
    blockNum = super[2];
    
    // set the inode to point to next empty block, set the size
    block[2] = blockNum;
    // stores the size as a string of characters.
    sprintf(block + 13, "%c", size);

    writeBlock(mounted, inode, block);

    // find the max amount of blocks to use
    int until = size / (BLOCKSIZE - 4) + 1;
    int i;
    // write all the blocks
    for (i = 0; i < until; i++) {
        readBlock(mounted, blockNum, block);
        message = calloc(sizeof(char), 256);
        
        message[0] = 3;
        message[1] = 'D';
        // take the first BLKSIZE - 4 and put it in a block
        if (i != until - 1) {
            message[2] = block[2];
            block_size = BLOCKSIZE - 4;
        } else {
            // if the last block, don't write another location
            // only write as much as needed.
            message[2] = 0;
            block_size = size % (BLOCKSIZE - 4);
        }

        strncpy(message + 4, buffer + (i*(BLOCKSIZE-4)), block_size); 
        
        // write that block
        writeBlock(mounted, blockNum, message);
        free(message);
        blockNum = block[2];
    }
    // set the free list to the last pointed free
    super[2] = blockNum;
    writeBlock(mounted, 0, super);

    

    return 0;
}

int tfs_deleteFile(fileDescriptor FD){
    // get blocks, this is same as writefile
    uint8_t block[BLOCKSIZE] = "";
    uint8_t super[BLOCKSIZE] = "";
    uint8_t * empty = calloc(sizeof(char), 256);
    int nextBlock, inode, curr, former;
    resNode *n;
    n = searchListFD(fileTable, FD);
    inode = n->fd;
    if (inode == NULL) {
        return -1;
        //error
    }
    readBlock(mounted, inode, block);
    readBlock(mounted, 0, super);

    //changes here. incrementally remove blocks associated to a file.
    curr = inode;
    former = super[2];

    super[2] = inode;
    // the inode is now free. It is inserted at the start of the list
    writeBlock(mounted, 0, super);
    nextBlock = block[2];

    // setup empty blocks
    empty[0] = 4;
    empty[1] = 'D';
    
    // while the end of the block chain isn't reached (designated by next block = 0)
    while (block[2] != 0) {
        // choose the next block in the chain
        empty[2] = nextBlock;
        // push empty onto the current block
        writeBlock(mounted, curr, empty);
        // read the next block
        readBlock(mounted, nextBlock, block);
        // sets new block to current
        curr = nextBlock;
        // makes the next block the block pointed to by the just read block
        nextBlock = block[2];
        
    }
    // sets the former front of the empty list to the end of the just added empty block
    empty[2] = former;
    writeBlock(mounted, curr, empty);
    //free the calloc buffer.
    free(empty);

    // this goes all the way to unmount in the sample diana sent, and in what I wrote
    tfs_closeFile(FD);
    return 0;
}

int tfs_readByte(fileDescriptor FD, char *buffer){
    // same setup as write and delete
    uint8_t block[BLOCKSIZE] = "";
    int nextBlock, inode, i, former, fp, bnum;
    resNode *n;
    n = searchListFD(fileTable, FD);
    inode = n->fd;
    fp = n->fp;
    if (inode == NULL) {
        return -1;
        //error
    }
    readBlock(mounted, inode, block);

    // calculate which block to go to, bnum is the block in 
    // the list, fp is the index in the block.
    bnum = fp / (BLOCKSIZE - 4);
    fp = fp - (bnum * (BLOCKSIZE - 4));

    // load up the block by incrementally passing through them
    for (i = 0; i < bnum + 1;i++) {
        nextBlock = block[2];
        readBlock(mounted, nextBlock, block);
    }
    // copy the exact location from the fp and the block offset, put it in buffer
    strncpy(buffer, block + 4 + fp, 1);
    // increment the fp
    n->fp++;
    return 0; 
}

int tfs_seek(fileDescriptor FD, int offset){
    // same setup as the other 3.
    uint8_t block[BLOCKSIZE] = "";
    int nextBlock, inode, i, former, fp, bnum;
    resNode *n;
    n = searchListFD(fileTable, FD);
    inode = n->fd;
    fp = n->fp;
    if (inode == NULL) {
        return -1;
        //error
    }
    // sets the fp to the offset
    n->fp = offset;
    return 0;
}

// returns the block where the file inode is (if it exists)
int checkFileExists(char * name) {
    int i;
    int diskSize = lseek(mounted, 0, SEEK_END) / 256;
    // buffer for grabbing blocks
    uint8_t buffer[256], check[8];
    for (i = 1; i < diskSize; i++) {
        // read block into the buffer
        readBlock(mounted, i, buffer);
        // if the block type is inode
        if (buffer[0] == 2) {
            // copy the filename into check
            strncpy(check, buffer[4], 8);
            // if its the same name, return it
            if (strcmp(name, check) == 0) return i;
        }
    }
    return 0;
}
