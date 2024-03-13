#define BLOCKSIZE 256

typedef struct block {
    char *filename;
    char *disk;
} block;

int openDisk(char *filename, int nBytes) {
    if (nBytes == 0) {
        exit(1);
        // return an existing disk with the filename value.
    }
    if (nBytes < BLOCKSIZE) {
        exit(1);
        // RETURN FAILURE
    }
    // loop through existing disks, and check filenames.
    // if its the same, replace it at that index.
    // if there are none of the same, make a new disk.

}
int closeDisk(int disk) {
    // free up the index given by the value.

}
int readBlock(int disk, int bNum, void *block) {
    // read byte offsets, 
}
int writeBlock(int disk, int bNum, void *block);