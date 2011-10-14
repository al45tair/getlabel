# A simple Makefile for the command line labelling tools

CFLAGS=-W -Wall -O
LDLIBS=-framework Carbon
INSTALL_DIR=/usr/local/bin
INSTALL_MAN_DIR=/usr/local/share/man/man1
INSTALL=/usr/bin/install

.PHONY:	all clean

all:		getlabel setlabel

clean:
		$(RM) *.o getlabel setlabel

install:	getlabel setlabel
		$(INSTALL) -m 0755 getlabel setlabel svn-save-labels \
				svn-restore-labels $(INSTALL_DIR)
		$(INSTALL) -m 0644 getlabel.1 setlabel.1 $(INSTALL_MAN_DIR)

getlabel:	getlabel.o
		$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

setlabel:	setlabel.o
		$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@
