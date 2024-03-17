#ifndef TINYFSERROR
#define TINYFSERROR

//TODO: 
// You must specify a set of unified error codes returned by your TinyFS interfaces. All error codes
// must be negative integers (-1 or lower), but it is up to you to assign specific meaning to each.
// Error codes must be informational only, and not used as status in subsequent conditionals. Create
// a file called tinyFS_errno.h and implement the codes as a set of statically defined macros.
// Take a look at man 3 errno on the UNIX* machines for examples of the types of errors you
// might catch and report.

#define	EDISKSIZE	  -1 /* Invalid disk size	*/
#define	EFILENAME	  -2 /* Invalid file name	*/
#define EFILENOTEXIST -3 /* File does not exist	*/
#define EFILECREATE   -4 /* Failed to create the file	*/
#define EDISKNUMBER   -5 /* Invalid disk number   */
#define EDISKCLOSE    -6 /* Failed to close the disk   */
#define EBLOCKNUMBER  -7 /* Invalid block number   */
#define EBLOCKNULL    -8 /* NULL block   */
#define EDISKSEEK     -9 /* File seek failed   */
#define EBLOCKREAD    -10 /* Block read failed   */
#define EBLOCKWRITE   -11 /* Block write failed   */
#define EDISKWRITE    -12 /* Disk write failed   */
#define EINVALIDTFS   -13 /* Invalid Mount   */
#define EMOUNTED      -14 /* TFS already mounted*/
#define EUNMOUNTED    -15 /* TFS already mounted*/

#endif