#include "combat.h"
#include "player.h"

void Combat_PlayFinisher(int finisher_id){
    switch(finisher_id){
        case FINISHER_BACK_SILENT:
            // เรียกแอนิเมชัน ท่าจากด้านหลังแบบเงียบ
            break;
        case FINISHER_BACK_FAST:
            // เรียกแอนิเมชัน ท่าจากด้านหลังแบบเร่งด่วน
            break;
        case FINISHER_SIDE:
            // เรียกแอนิเมชัน ท่าจากด้านข้าง
            break;
        case FINISHER_GRASS_HIDE:
            // เรียกแอนิเมชัน ดึงศัตรูลงกองหญ้า
            break;
    }
    Combat_PostKillLookAround();
}

void Combat_PostKillLookAround(){
    Player_SetAnim(ANIM_LOOK_AROUND);
}

int Combat_CheckCanSilentKill(float px,float pz,float nx,float nz,float nrot){
    float dist = sqrtf( (px‑nx)*(px‑nx)+(pz‑nz)*(pz‑nz) );
    if(dist > 2.0f) return 0; // ไกลเกินไปทำไม่ได้
    // ตรวจมุมว่าอยู่ด้านหลังศัตรู
    return 1; // คืนค่า 1 = สามารถฆ่าเงียบได้ (ปรับละเอียดมุมเองต่อ)
}
