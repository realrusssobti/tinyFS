#ifndef TINYFSERROR
#define TINYFSERROR

//TODO: 
// You must specify a set of unified error codes returned by your TinyFS interfaces. All error codes
// must be negative integers (-1 or lower), but it is up to you to assign specific meaning to each.
// Error codes must be informational only, and not used as status in subsequent conditionals. Create
// a file called tinyFS_errno.h and implement the codes as a set of statically defined macros.
// Take a look at man 3 errno on the UNIX* machines for examples of the types of errors you
// might catch and report.

#define	EDISKSIZE	-1	/* Invalid disk size	*/
#define	EFILENAME	-2	/* Invalid file name	*/
#define EFILENOTEXIST -3 /* File does not exist	*/
#define EFILECREATE -4 /* Failed to create the file	*/

#endif