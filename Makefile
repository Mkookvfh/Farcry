# ชื่อไฟล์ ELF ผลลัพธ์ของเกม
EE_BIN = outpost_island.elf

# รายชื่อไฟล์โค้ดหลัก (เช่น ถ้ามีไฟล์ main.c ให้ใส่ main.o)
EE_OBJS = main.o

# ไลบรารีมาตรฐานสำหรับใช้กราฟิกและระบบของ PS2
EE_LIBS = -lgraph -ldraw -lpacket -ldma -lmgr -lc

all: $(EE_BIN)

clean:
	rm -f *.o $(EE_BIN)

# เชื่อมโยงกับชุดพัฒนา PS2SDK มาตรฐาน
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.rules
