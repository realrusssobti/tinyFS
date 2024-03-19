makeCC = gcc
CFLAGS = -Wall -g
PROG = tinyFSDemo
OBJS = tinyFSDemo.o libTinyFS.o libDisk.o linkedList.o

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

diskTest.o: diskTest.c libDisk.h libDisk.o
	$(CC) $(CFLAGS) -c -o $@ $<

diskTest: diskTest.o libDisk.o
	$(CC) $(CFLAGS) -o diskTest diskTest.o libDisk.o

frag_test.o: frag_test.c libTinyFS.h tinyFS.h TinyFS_errno.h
	$(CC) $(CFLAGS) -c -o $@ $<
frag_test: frag_test.o libTinyFS.o libDisk.o linkedList.o
	$(CC) $(CFLAGS) -o frag_test frag_test.o libTinyFS.o libDisk.o linkedList.o

tfsTest.o: tfsTest.c libTinyFS.h tinyFS.h TinyFS_errno.h
	$(CC) $(CFLAGS) -c -o $@ $<
tfsTest: tfsTest.o libTinyFS.o libDisk.o linkedList.o

all: tinyFSDemo diskTest frag_test tfsTest
clean:
	rm -f $(OBJS) tinyFSDemo
