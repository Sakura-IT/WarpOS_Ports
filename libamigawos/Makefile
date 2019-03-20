CC = gcc --specs=warpup
AR = ar
DEBUG = 0
OSTYPE = $(shell uname -s)

CFLAGS = -I.

ifneq ($(DEBUG), 0)
CFLAGS += -g -DDEBUG
else
CFLAGS += -O3
endif

#ifeq ($(OSTYPE), MorphOS)
#CFLAGS += -DAROS_ALMOST_COMPATIBLE -D_POSIX_THREADS
#endif

#ifeq ($(OSTYPE), AROS)
# I shouldn't need to do this manually
#CFLAGS += -march=i586
#endif

OBJ = beginio.o createio.o createport.o argarray.o createtask.o \
      fastrand.o hotkey.o rangerand.o timedelay.o waitbeam.o \
      sysconst.o

all: libamigawos.a

libamigawos.a: $(OBJ)
	$(AR) rcs $@ $?

clean:
	rm -f $(OBJ)
