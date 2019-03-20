CC = gcc --specs=warpup
AR = ar
DEBUG = 0
OSTYPE = $(shell uname -s)

CFLAGS = -Wall -I.

ifneq ($(DEBUG), 0)
CFLAGS += -g -DDEBUG
else
CFLAGS += -O2
endif

ifeq ($(OSTYPE), MorphOS)
CFLAGS += -DAROS_ALMOST_COMPATIBLE -D_POSIX_THREADS
endif

ifeq ($(OSTYPE), AROS)
# I shouldn't need to do this manually
CFLAGS += -march=i586
endif

OBJ = \
	pthread.o \
	sched.o \
	semaphore.o

all: libpthread.a

libpthread.a: $(OBJ)
	$(AR) rcs $@ $?

clean:
	rm -f $(OBJ)
