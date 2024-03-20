CC = gcc
CFLAGS = -Wall -g
BFISH = $(shell grep '^CFLAGS = ' crypt_blowfish/Makefile | cut -d= -f2-)
PROG = tinyFSDemo
OBJS = tinyFSDemo.o libTinyFS.o libDisk.o linkedList.o aes.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

tinyFSDemo.o: tinyFSDemo.c libTinyFS.h tinyFS.h TinyFS_errno.h 
	$(CC) $(CFLAGS) -c -o $@ $<

libTinyFS.o: libTinyFS.c libTinyFS.h tinyFS.h libDisk.h libDisk.o TinyFS_errno.h
	$(CC) $(CFLAGS) -c -o $@ $<

libDisk.o: libDisk.c libDisk.h tinyFS.h TinyFS_errno.h
	$(CC) $(CFLAGS) -c -o $@ $<

linkedList.o: linkedList.c linkedList.h TinyFS_errno.h libTinyFS.h
	$(CC) $(CFLAGS) -c -o $@ $<

aes.o: aes.c aes.h
	$(CC) $(CFLAGS) -c -o $@ $<

diskTest.o: diskTest.c libDisk.h libDisk.o
	$(CC) $(CFLAGS) -c -o $@ $<

diskTest: diskTest.o libDisk.o
	$(CC) $(CFLAGS) -o diskTest diskTest.o libDisk.o

clean:
	rm -f $(OBJS) tinyFSDemo
