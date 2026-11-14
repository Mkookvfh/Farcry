#include "player.h"

Player player;

void Player_Init(){
    player.x=0.0f;
    player.y=0.0f;
    player.z=0.0f;
    player.rot=0.0f;
    player.anim_state=ANIM_IDLE;
    player.is_crouch=0;
    player.is_climbing=0;
}

void Player_SetAnim(int anim){
    player.anim_state=anim;
}

void Player_Update(float delta){
    // ตัวอย่างรับอินพุตจอย PS2 (คุณต้องเติมโค้ดอ่านจอยจริงจากไลบรารี PS2 SDK)
    float move_x=0.0f, move_z=0.0f;
    float speed_walk=1.2f;
    float speed_run=3.0f;
    float speed_crouch=0.6f;

    if(player.is_climbing){
        Player_SetAnim(ANIM_CLIMB);
    }else if(player.is_crouch){
        Player_SetAnim(ANIM_CROUCH);
    }else if(move_x!=0 || move_z!=0){
        // ตรวจว่ากดวิ่งหรือไม่
        // ถ้ากดปุ่มวิ่ง -> ANIM_RUN ไม่กด -> ANIM_WALK
        Player_SetAnim(ANIM_WALK);
    }else{
        Player_SetAnim(ANIM_IDLE);
    }

    player.x += move_x * delta;
    player.z += move_z * delta;
}
