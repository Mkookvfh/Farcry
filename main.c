/*
 * Assassin's Creed inspired Side-View Parkour + Stealth
 * PS2 Homebrew (ps2sdk + gsKit)
 * Features: Parkour, Stealth, Assassination, Improved Physics, AC-style character
 */

#include <stdio.h>
#include <tamtypes.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <math.h>
#include <string.h>

#define SCREEN_W 640
#define SCREEN_H 448
#define TILE 32
#define MAP_W 90
#define MAP_H 18

GSGLOBAL *gsGlobal;

unsigned char map[MAP_H][MAP_W];

typedef struct {
    float x, y;
    float vx, vy;
    int onGround;
    int onWall;
    int climbing;
    int facing;
    int hidden;
    int canAssassinate;
    int state;          // 0 idle, 1 run, 2 jump, 3 climb
    int animFrame;
    int animTimer;
} Player;

typedef struct {
    float x, y;
    float vx;
    int alive;
    int facing;
    int patrolMin, patrolMax;
} Enemy;

Player P;
Enemy E[8];
int enemyCount = 0;
float camX = 0.0f;

static char padBuf[256] __attribute__((aligned(64)));

void loadModules() {
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);
}

void initPad() {
    padInit(0);
    padPortOpen(0, 0, padBuf);
}

int readPad(struct padButtonStatus *btns) {
    return padRead(0, 0, btns) != 0;
}

void playSfx(int id) {
    // ยังไม่มีไฟล์เสียงจริง
    // id 0 = กระโดด, 1 = ลอบสังหาร, 2 = เดิน
    (void)id;
}

void generateMap() {
    memset(map, 0, sizeof(map));

    for(int x = 0; x < MAP_W; x++) {
        map[MAP_H-1][x] = 1;
        map[MAP_H-2][x] = 1;
    }

    // อาคาร + กำแพงปีน + เงา
    for(int x = 3; x < 14; x++) map[13][x] = 1;
    for(int y = 13; y < 16; y++) { map[y][3] = 2; map[y][13] = 2; }
    for(int x = 5; x < 9; x++) map[14][x] = 3;

    for(int x = 18; x < 32; x++) map[10][x] = 1;
    for(int y = 10; y < 16; y++) { map[y][18] = 2; map[y][31] = 2; }
    for(int x = 22; x < 28; x++) map[11][x] = 3;

    for(int x = 36; x < 42; x++) map[12][x] = 1;
    for(int x = 46; x < 55; x++) map[8][x] = 1;
    for(int y = 8; y < 15; y++) map[y][46] = 2;

    for(int x = 60; x < 78; x++) map[6][x] = 1;
    for(int y = 6; y < 14; y++) { map[y][60] = 2; map[y][77] = 2; }
    for(int x = 65; x < 73; x++) map[7][x] = 3;

    // Ledges
    map[14][15] = 4; map[14][16] = 4;
    map[11][33] = 4; map[11][34] = 4;
    map[9][43] = 4;  map[9][44] = 4;
    map[7][56] = 4;  map[7][57] = 4;
}

void initGS() {
    gsGlobal = gsKit_init_global();
    gsGlobal->Mode = GS_MODE_NTSC;
    gsGlobal->Interlace = GS_INTERLACED;
    gsGlobal->Field = GS_FIELD;
    gsGlobal->Width = SCREEN_W;
    gsGlobal->Height = SCREEN_H;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsKit_init_screen(gsGlobal);
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
}

int solid(int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return 1;
    return map[ty][tx] == 1 || map[ty][tx] == 4;
}

int climbable(int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return 0;
    return map[ty][tx] == 2;
}

int isShadow(int tx, int ty) {
    if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return 0;
    return map[ty][tx] == 3;
}

void updatePlayer(struct padButtonStatus *btns) {
    u32 btn = 0xffff ^ btns->btns;

    float accel     = 0.48f;
    float maxSpeed  = 2.65f;
    float friction  = 0.82f;
    float jumpForce = -8.1f;
    float gravity   = 0.36f;
    float maxFall   = 10.5f;

    // Horizontal
    if (btn & PAD_LEFT) {
        P.vx -= accel;
        P.facing = -1;
        P.state = 1;
    } else if (btn & PAD_RIGHT) {
        P.vx += accel;
        P.facing = 1;
        P.state = 1;
    } else {
        P.vx *= friction;
        if (fabsf(P.vx) < 0.12f) P.vx = 0;
        if (P.onGround) P.state = 0;
    }

    if (P.vx > maxSpeed) P.vx = maxSpeed;
    if (P.vx < -maxSpeed) P.vx = -maxSpeed;

    // Jump
    if ((btn & PAD_CROSS) && P.onGround) {
        P.vy = jumpForce;
        P.onGround = 0;
        P.state = 2;
        playSfx(0);
    }

    // Climb
    int left = (int)(P.x) / TILE;
    int right = (int)(P.x + 16) / TILE;
    int midY = (int)(P.y + 16) / TILE;

    P.onWall = climbable(left-1, midY) || climbable(right+1, midY) ||
               climbable(left-1, midY-1) || climbable(right+1, midY-1);

    if (P.onWall && (btn & PAD_UP)) {
        P.climbing = 1;
        P.vy = -2.35f;
        P.vx *= 0.25f;
        P.state = 3;
    } else if (P.climbing && (btn & PAD_DOWN)) {
        P.vy = 2.15f;
        P.state = 3;
    } else {
        P.climbing = 0;
    }

    // Gravity
    if (!P.climbing) {
        P.vy += gravity;
        if (P.vy > maxFall) P.vy = maxFall;
    }

    P.x += P.vx;
    P.y += P.vy;

    left = (int)(P.x) / TILE;
    right = (int)(P.x + 16) / TILE;
    int top = (int)(P.y) / TILE;
    int bottom = (int)(P.y + 32) / TILE;
    midY = (int)(P.y + 16) / TILE;

    // Ground
    P.onGround = 0;
    if (solid(left, bottom) || solid(right, bottom)) {
        P.y = bottom * TILE - 32;
        P.vy = 0;
        P.onGround = 1;
        P.climbing = 0;
    }

    // Ceiling
    if (solid(left, top) || solid(right, top)) {
        P.y = (top + 1) * TILE;
        P.vy = 0;
    }

    // Side collision
    if (P.vx > 0 && (solid(right, top) || solid(right, midY))) {
        P.x = right * TILE - 17;
        P.vx = 0;
    }
    if (P.vx < 0 && (solid(left, top) || solid(left, midY))) {
        P.x = (left + 1) * TILE;
        P.vx = 0;
    }

    // Ledge assist
    if (!P.onGround && P.vy > 0.6f && P.vy < 4.8f) {
        if (solid(left, bottom-1) || solid(right, bottom-1)) {
            P.vy -= 1.1f;
        }
    }

    // Shadow
    P.hidden = isShadow(left, bottom) || isShadow(right, bottom) || isShadow(left, midY);

    // Assassination
    P.canAssassinate = 0;
    for(int i = 0; i < enemyCount; i++) {
        if (!E[i].alive) continue;
        float dx = fabsf(P.x - E[i].x);
        float dy = fabsf(P.y - E[i].y);
        if (dx < 32 && dy < 44) {
            if (P.hidden || (P.y + 14 < E[i].y)) {
                P.canAssassinate = 1;
                if (btn & PAD_CIRCLE) {
                    E[i].alive = 0;
                    playSfx(1);
                }
            }
        }
    }

    // Animation
    P.animTimer++;
    if (P.animTimer > 5) {
        P.animTimer = 0;
        P.animFrame = (P.animFrame + 1) % 4;
    }

    // Camera
    camX = P.x - SCREEN_W * 0.36f;
    if (camX < 0) camX = 0;
    if (camX > MAP_W * TILE - SCREEN_W) camX = MAP_W * TILE - SCREEN_W;
}

void updateEnemies() {
    for(int i = 0; i < enemyCount; i++) {
        if (!E[i].alive) continue;
        E[i].x += E[i].vx;
        if (E[i].x < E[i].patrolMin || E[i].x > E[i].patrolMax) {
            E[i].vx = -E[i].vx;
            E[i].facing = -E[i].facing;
        }
    }
}

void drawTile(int tx, int ty, int sx, int sy) {
    u64 c;
    switch(map[ty][tx]) {
        case 1: c = GS_SETREG_RGBAQ(96, 74, 52, 0x80, 0x00); break;
        case 2: c = GS_SETREG_RGBAQ(76, 60, 46, 0x80, 0x00); break;
        case 3: c = GS_SETREG_RGBAQ(22, 26, 40, 0x80, 0x00); break;
        case 4: c = GS_SETREG_RGBAQ(122, 98, 68, 0x80, 0x00); break;
        default: return;
    }
    gsKit_prim_sprite(gsGlobal, sx, sy, sx+TILE, sy+TILE, 1, c);
}

void drawCharacter(float x, float y, int facing, int state, int frame, int hidden, int highlight) {
    int px = (int)(x - camX);
    int py = (int)y;

    u64 hood   = hidden ? GS_SETREG_RGBAQ(25, 30, 45, 0x80, 0x00) : GS_SETREG_RGBAQ(35, 40, 55, 0x80, 0x00);
    u64 cloak  = hidden ? GS_SETREG_RGBAQ(20, 25, 38, 0x80, 0x00) : GS_SETREG_RGBAQ(30, 35, 50, 0x80, 0x00);
    u64 body   = hidden ? GS_SETREG_RGBAQ(40, 48, 65, 0x80, 0x00) : GS_SETREG_RGBAQ(55, 65, 90, 0x80, 0x00);
    u64 skin   = hidden ? GS_SETREG_RGBAQ(60, 55, 50, 0x80, 0x00) : GS_SETREG_RGBAQ(180, 140, 110, 0x80, 0x00);
    u64 accent = GS_SETREG_RGBAQ(140, 40, 40, 0x80, 0x00);

    if (highlight) {
        body  = GS_SETREG_RGBAQ(160, 50, 45, 0x80, 0x00);
        cloak = GS_SETREG_RGBAQ(120, 35, 35, 0x80, 0x00);
    }

    int dir = (facing > 0) ? 1 : -1;
    int legSwing = 0;
    int armSwing = 0;

    if (state == 1) {
        legSwing = (frame % 2) ? 5 : -5;
        armSwing = (frame % 2) ? -3 : 3;
    } else if (state == 2) {
        legSwing = 4;
        armSwing = -4;
    } else if (state == 3) {
        legSwing = (frame % 2) ? 4 : -4;
        armSwing = (frame % 2) ? 3 : -3;
    }

    // Cloak
    int cloakX = px + (dir > 0 ? 2 : 6);
    gsKit_prim_sprite(gsGlobal, cloakX, py + 6, cloakX + 12, py + 36, 1, cloak);

    // Hood
    gsKit_prim_sprite(gsGlobal, px + 3, py - 2, px + 15, py + 8, 2, hood);
    if (dir > 0)
        gsKit_prim_sprite(gsGlobal, px + 12, py + 1, px + 17, py + 7, 2, hood);
    else
        gsKit_prim_sprite(gsGlobal, px + 1, py + 1, px + 6, py + 7, 2, hood);

    // Body
    gsKit_prim_sprite(gsGlobal, px + 5, py + 8, px + 13, py + 24, 3, body);
    gsKit_prim_sprite(gsGlobal, px + 5, py + 20, px + 13, py + 22, 3, accent);

    // Arms
    gsKit_prim_sprite(gsGlobal, px + 3, py + 10, px + 6, py + 19 + armSwing, 3, body);
    gsKit_prim_sprite(gsGlobal, px + 12, py + 10, px + 15, py + 19 - armSwing, 3, body);

    // Legs
    gsKit_prim_sprite(gsGlobal, px + 5, py + 24, px + 8, py + 36, 3, body);
    gsKit_prim_sprite(gsGlobal, px + 10, py + 24, px + 13, py + 36 + legSwing, 3, body);

    // Hand
    if (dir > 0 && state != 3) {
        gsKit_prim_sprite(gsGlobal, px + 14, py + 16, px + 18, py + 19, 3, skin);
    }
}

void draw() {
    gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(32, 42, 62, 0x00, 0x00));

    int startX = (int)camX / TILE - 1;
    int endX = startX + (SCREEN_W / TILE) + 3;

    for(int ty = 0; ty < MAP_H; ty++) {
        for(int tx = startX; tx < endX; tx++) {
            if (tx < 0 || tx >= MAP_W) continue;
            drawTile(tx, ty, tx * TILE - (int)camX, ty * TILE);
        }
    }

    drawCharacter(P.x, P.y, P.facing, P.state, P.animFrame, P.hidden, P.canAssassinate);

    for(int i = 0; i < enemyCount; i++) {
        if (!E[i].alive) continue;
        drawCharacter(E[i].x, E[i].y, E[i].facing, 1, 0, 0, 0);
    }

    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
}

int main(int argc, char *argv[]) {
    SifInitRpc(0);
    loadModules();
    initPad();
    initGS();
    generateMap();

    P.x = 80;
    P.y = 300;
    P.vx = P.vy = 0;
    P.facing = 1;
    P.state = 0;
    P.animFrame = 0;
    P.animTimer = 0;

    enemyCount = 5;
    E[0] = (Enemy){240, 280,  0.85f, 1, 1, 200, 310};
    E[1] = (Enemy){400, 210, -0.72f, 1, -1, 370, 460};
    E[2] = (Enemy){520, 170,  0.78f, 1, 1, 490, 580};
    E[3] = (Enemy){680, 120, -0.68f, 1, -1, 650, 740};
    E[4] = (Enemy){150, 320,  0.55f, 1, 1, 120, 200};

    struct padButtonStatus btns;

    while (1) {
        if (readPad(&btns)) {
            updatePlayer(&btns);
        }
        updateEnemies();
        draw();
    }
    return 0;
}