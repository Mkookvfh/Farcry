#include <stdio.h>
#include <math.h>
#include <gsKit.h>
#include <libpad.h>

// ===== การตั้งค่าหน้าจอ แก้จอดำสำหรับ NTSC 480i =====
#define SCREEN_W 640
#define SCREEN_H 448
#define WORLD_SIZE 800.0f

static GSGLOBAL *gsGlobal;

typedef struct {
	float x,y,z;
	float rot;
	float health;
	int alive;
} Entity;

Entity player;

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

void init_game(void)
{
	player.x = WORLD_SIZE/2;
	player.y = 2.0f;
	player.z = WORLD_SIZE/2;
	player.rot = 0;
	player.health = 100;
	player.alive = 1;
}

void update_game(void)
{
	// อัปเดตผู้เล่น ศัตรู สัตว์ป่า ยานพาหนะ ภารกิจ
}

void render_game(void)
{
	gsKit_clear(gsGlobal, 0xFF103050);
	// วาดแผนที่เกาะ ศัตรู สัตว์ ยานพาหนะ HUD ภาษาไทย
	gsKit_swapbuffers(gsGlobal);
}

int main(void)
{
	init_video();
	init_game();
	while(1)
	{
		update_game();
		render_game();
	}
	return 0;
}
