#include <stdio.h>
#include <draw.h>
#include <graph.h>
#include <dma.h>
#include <vsync.h>

#define WIDTH 640
#define HEIGHT 448

int main(void)
{
    int x, y;
    u32 *fb;

    setMode(MODE_NTSC);
    setFbMode(PSM_CT32, WIDTH, HEIGHT);
    fb = draw_get_fb();

    while(1)
    {
        for(y = 0; y < HEIGHT; y++)
        {
            for(x = 0; x < WIDTH; x++)
            {
                fb[y * WIDTH + x] = 0xFF003366;
            }
        }
        vsync_wait(1);
    }
    return 0;
}
