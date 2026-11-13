#include <stdio.h>
#include <math.h>
#include <gsKit.h>
#include <libpad.h>
#include <GL/gl.h>
#include <GL/ps2gl.h>

// ===== การตั้งค่าหน้าจอ แก้จอดำ NTSC‑480i =====
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

	// เริ่มต้น PS2GL สำหรับเรนเดอร์ 3D
	glPS2Init(SCREEN_W,SCREEN_H,0);
	glViewport(0,0,SCREEN_W,SCREEN_H);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
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
	// อัปเดต: ผู้เล่น, ศัตรู, สัตว์ป่า, คู่หู, ยานพาหนะ, ภารกิจ
}

void render_3d_world(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(player.x, player.y, player.z,
		player.x+sinf(player.rot), player.y, player.z+cosf(player.rot),
		0.0f,1.0f,0.0f);

	// วาดพื้นเกาะ
	glColor3f(0.2f,0.55f,0.2f);
	glBegin(GL_QUADS);
	glVertex3f(-WORLD_SIZE,0,-WORLD_SIZE);
	glVertex3f(WORLD_SIZE,0,-WORLD_SIZE);
	glVertex3f(WORLD_SIZE,0,WORLD_SIZE);
	glVertex3f(-WORLD_SIZE,0,WORLD_SIZE);
	glEnd();

	// ต่อเพิ่ม: ต้นไม้ ภูเขา ศัตรู สัตว์ รถ เรือ HUD ข้อความภาษาไทย
}

int main(void)
{
	init_video();
	init_game();
	while(1)
	{
		update_game();
		render_3d_world();
		glPS2SwapBuffers();
		gsKit_swapbuffers(gsGlobal);
	}
	return 0;
}
