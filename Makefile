EE_INCS += -I$(PS2SDK)/include
EE_LIBS += -lgsKit -ldraw -ldma -lgraph -lpad -lm -lps2gl
EE_CFLAGS += -O2 -DNTSC_480I
EE_BIN = TROPIC_ASSAULT_FIXED.ELF
EE_OBJS = main.o
all: $(EE_BIN)
clean:
	rm -f *.o $(EE_BIN)
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
