#include <stdio.h>
#include "player.h"
#include "combat.h"
#include "ai_npc.h"

NPCGuard guard;

int main(){
    Player_Init();
    guard.alive=1;
    guard.alert_level=0;

    float delta=0.016f; // ~60FPS

    while(1){
        Player_Update(delta);
        NPC_Update(&guard, player.x, player.z, delta);

        // ตัวอย่างทริกเกอร์ฆ่าเงียบ (กดปุ่มสมมติ)
        /*
        if( ปุ่มกด ){
            if(Combat_CheckCanSilentKill(player.x,player.z,guard.x,guard.z,guard.rot)){
                Combat_PlayFinisher(FINISHER_BACK_SILENT);
                guard.alive=0;
            }
        }
        */
        // Render (ใส่โค้ดเรนเดอร์จาก PS2 SDK ตรงนี้)
    }
    return 0;
}
