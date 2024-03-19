#ifndef LIBTINYFS
#define LIBTINYFS

#include "tinyFS.h"
#include "linkedList.h"

#include <time.h>
// These are the different kinds of blocks: SuperBlock, Inode, Data, Free
typedef struct superBlock{
	char Type;
	char magicNumber;
	char rootBlockNumber;
	char freeBlocksRoot;
	char empty[BLOCKSIZE - 4];

} superBlock;

typedef struct inode{
	char Type;
	char magicNumber;
	char name[FILENAME_LEN +1];
	char startBlock;
	char nextInode;
	char fp;
	time_t creationTime;
	time_t lastAccessTime;
	char empty[BLOCKSIZE - 14 - 2*sizeof (time_t)];
} inode;

typedef struct DataBlock{
	char Type;
	char magicNumber;
	char data[BLOCKSIZE - 3];
	char nextBlock;
} DataBlock;

typedef struct freeBlock{
	char Type;
	char magicNumber;
	char nextFreeBlock;
	char empty[BLOCKSIZE - 3];
} freeBlock;


// Dynamic resource table implemented as a singly linked list
typedef struct DynamicResourceTable{
   struct DynamicResourceTable *next;
   fileDescriptor fd;
   char name[FILENAME_LEN+1];
   time_t creationTime;
   time_t lastAccessTime;
} DynamicResourceTable;

typedef struct resourceTableNode{
   fileDescriptor fd;
   int inodeIndex;
   char name[FILENAME_LEN];
   int fp;
} resNode;

/* Makes a blank TinyFS file system of size nBytes on the unix file
specified by ‘filename’. This function should use the emulated disk
library to open the specified unix file, and upon success, format the
file to be a mountable disk. This includes initializing all data to 0x00,
setting magic numbers, initializing and writing the superblock and
inodes, etc. Must return a specified success/error code. */
int tfs_mkfs(char *filename, int nBytes);

/* tfs_mount(char *diskname) “mounts” a TinyFS file system located within
‘diskname’. tfs_unmount(void) “unmounts” the currently mounted file
system. As part of the mount operation, tfs_mount should verify the file
system is the correct type. In tinyFS, only one file system may be
mounted at a time. Use tfs_unmount to cleanly unmount the currently
mounted file system. Must return a specified success/error code. */
int tfs_mount(char *diskname);
int tfs_unmount(void);

/* Creates or Opens a file for reading and writing on the currently
mounted file system. Creates a dynamic resource table entry for the file,
and returns a file descriptor (integer) that can be used to reference
this entry while the filesystem is mounted. */
fileDescriptor tfs_openFile(char *name);

/* Closes the file, de-allocates all system resources, and removes table
entry */
int tfs_closeFile(fileDescriptor FD);

/* Writes buffer ‘buffer’ of size ‘size’, which represents an entire
file’s content, to the file system. Previous content (if any) will be
completely lost. Sets the file pointer to 0 (the start of file) when
done. Returns success/error codes. */
int tfs_writeFile(fileDescriptor FD,char *buffer, int size);

/* deletes a file and marks its blocks as free on disk. */
int tfs_deleteFile(fileDescriptor FD);

/* reads one byte from the file and copies it to buffer, using the
current file pointer location and incrementing it by one upon success.
If the file pointer is already past the end of the file then
tfs_readByte() should return an error and not increment the file pointer.
*/
int tfs_readByte(fileDescriptor FD, char *buffer);

/* change the file pointer location to offset (absolute). Returns
success/error codes.*/
int tfs_seek(fileDescriptor FD, int offset);


int checkFileExists(char * name);
#endif

void tfs_displayFragments(void);
void tfs_defrag(void);