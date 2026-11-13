EE_INCS += -I$(PS2SDK)/include -I$(PS2SDK)/ports/include -I/usr/local/ps2dev/gsKit/include
EE_LIBS += -lgsKit -ldraw -ldma -lgraph -lpad -lm
EE_CFLAGS += -O2 -DNTSC_480I
EE_BIN = TROPIC_TEST.ELF
EE_OBJS = main.o
all: $(EE_BIN)
clean:
	rm -f *.o $(EE_BIN)
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
