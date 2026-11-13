#include <stdio.h>
#include <gsKit.h>

#define SCREEN_W 640
#define SCREEN_H 448

static GSGLOBAL *gsGlobal;

void init_video(void)
{
	gsGlobal = gsKit_init_global();
	gsGlobal->Mode = GS_MODE_NTSC;
	gsGlobal->Width = SCREEN_W;
	gsGlobal->Height = SCREEN_H;
	gsGlobal->Interlace = GS_INTERLACED;
	gsGlobal->Field = GS_FIELD;
	gsKit_init(gsGlobal);
	gsKit_set_display(gsGlobal);
}

int main(void)
{
	init_video();
	while(1)
	{
		// สีพื้นสีน้ำเงินทดสอบว่าแสดงภาพได้
		gsKit_clear(gsGlobal, 0xFF003366);
		gsKit_swapbuffers(gsGlobal);
	}
	return 0;
}
