# ชื่อไฟล์ ELF ผลลัพธ์ของเกม PS2
EE_BIN = outpost_island.elf

# ไฟล์ C หลักของโปรเจกต์ (ถ้ามีไฟล์ main.c ให้ใช้ main.o)
EE_OBJS = main.o

# ไลบรารีพื้นฐานของระบบ PS2
EE_LIBS = -lkernel

all: $(EE_BIN)

clean:
	rm -f *.o $(EE_BIN)

# ดึงค่าตั้งค่ามาตรฐานจาก PS2SDK
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
