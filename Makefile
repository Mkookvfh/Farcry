# ชื่อไฟล์ ELF ผลลัพธ์ที่จะได้
EE_BIN = outpost_island.elf

# รายชื่อไฟล์ Object (.o) ที่เกิดจากการคอมไพล์ซอร์สโค้ดของคุณ
# (ถ้ามีไฟล์ main.c ให้ใส่ main.o, ถ้ามี game.c ให้ใส่ game.o เพิ่มเข้าไป)
EE_OBJS = game.o graphics.o 

# ไดเรกทอรีหรือไลบรารีเสริมที่ต้องการใช้งาน (ถ้ามี)
EE_LIBS = -lgraph -ldraw -lpacket -ldma -lmgr -lc

# แฟล็กสำหรับคอมไพเลอร์ (เพิ่มเติมได้ตามต้องการ)
EE_CFLAGS = -O2 -G0 -Wall
EE_LDFLAGS = 

all: $(EE_BIN)

# คำสั่งทำความสะอาดไฟล์ที่คอมไพล์แล้ว
clean:
	rm -f *.o $(EE_BIN)

# รวมการตั้งค่ามาตรฐานของ PS2SDK
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
include $(PS2SDK)/samples/Makefile.rules
