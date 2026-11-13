#include <stdio.h>
#include <draw.h>
#include <graph.h>
#include <dma.h>

#define WIDTH   640
#define HEIGHT  448

static u64 *frameBuf;

void initVideo(void)
{
	// ตั้งโหมด NTSC 480i
	setMode(MODE_NTSC);
	setFbMode(PSM_CT32, WIDTH, HEIGHT);
	frameBuf = getFrameBuffer();
}

int main(void)
{
	initVideo();

	while(1)
	{
		// เต็มหน้าจอด้วยสีน้ำเงินทดสอบ
		int x,y;
		for(y=0;y<HEIGHT;y++){
			for(x=0;x<WIDTH;x++){
				frameBuf[y*WIDTH + x] = 0xFF003366ULL;
			}
		}
		vsyncWait();
	}
	return 0;
}
