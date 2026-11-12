# ชื่อไฟล์ ELF ผลลัพธ์
EE_BIN = outpost_island.elf

# รายชื่อไฟล์ Object
EE_OBJS = main.o

# ไลบรารีหลักของ PS2
EE_LIBS = -lkernel

all: $(EE_BIN)

clean:
	rm -f *.o $(EE_BIN)

# ใช้ค่ามาตรฐานของ ps2sdk โดยตรง (รองรับ ps2dev ยุคใหม่)
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
