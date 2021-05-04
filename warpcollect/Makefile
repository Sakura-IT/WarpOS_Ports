CC = gcc
CFLAGS = -Wall -O3 -fomit-frame-pointer -D_LINUX_

LIBS =

EXE = warpcollect
OBJS = warpcollect.o

INSTALLDIR = /gg/bin

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXE)

install:
	cp $(EXE) $(INSTALLDIR)/$(EXE)
