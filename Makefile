EE_BIN = assassin_ps2.elf
EE_OBJS = src/main.o src/player.o src/combat.o src/ai_npc.o
EE_LIBS = ‑lmath

all: $(EE_BIN)

clean:
	rm ‑f $(EE_OBJS) $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
