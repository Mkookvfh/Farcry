EE_BIN = TROPIC_TEST.ELF
EE_OBJS = main.o

EE_LIBS += -ldraw -ldma -lgraph -lpad -lm
EE_CFLAGS += -O2

all: $(EE_BIN)
	$(EE_STRIP) $(EE_BIN)

clean:
	rm -f *.o $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
