#include "World.h"

World::World()
{
   initGX();
   initChunkArray();

   pitch = 0.0f; yaw = 3.1416f;
   camera = new Camera(&position, &pitch, &yaw);
}

World::~World()
{
   deleteChunkArray();
   exitGX();
}


Chunk *World::getChunkAt(int x, int y, int z)
{
   if( (x < 0 || x >= WORLD_LENGTH) || (y < 0 || y >= WORLD_HEIGHT) || (z < 0 || z >= WORLD_LENGTH)) {
      return NULL;
   }
   return chunkArray[x + y * WORLD_LENGTH + z * WORLD_LENGTH * WORLD_HEIGHT];
}


void World::initChunkArray()
{
    Chunk *topC, *bottomC, *rightC, *leftC, *frontC, *backC;
    Chunk *current;

    int x, y, z;
    for(z = 0; z < WORLD_LENGTH; z++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            for(x = 0; x < WORLD_LENGTH; x++) {
                chunkArray[x + y * WORLD_LENGTH + z * WORLD_LENGTH * WORLD_HEIGHT] = new Chunk(x, y, z);
            }
        }
    }

    for(z = 0; z < WORLD_LENGTH; z++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            for(x = 0; x < WORLD_LENGTH; x++) {
                topC    = getChunkAt(x, y + 1, z);
                bottomC = getChunkAt(x, y - 1, z);
                rightC  = getChunkAt(x + 1, y, z);
                leftC   = getChunkAt(x - 1, y, z);
                frontC  = getChunkAt(x, y, z + 1);
                backC   = getChunkAt(x, y, z - 1);

                current = getChunkAt(x, y, z);
                current->setNeighbours(topC, bottomC, rightC, leftC, frontC, backC);
                current->rebuildCubeRenderList();
            }
        }
    }
}

void World::deleteChunkArray()
{
   int x, y, z;
    for(z = 0; z < WORLD_LENGTH; z++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            for(x = 0; x < WORLD_LENGTH; x++) {
                delete chunkArray[x + y * WORLD_LENGTH + z * WORLD_LENGTH * WORLD_HEIGHT];
         }
      }
   }
}



void World::drawChunks()
{
    int x, y, z;
    for(z = 0; z < WORLD_LENGTH; z++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            for(x = 0; x < WORLD_LENGTH; x++) {
                chunkArray[x + y * WORLD_LENGTH + z * WORLD_LENGTH * WORLD_HEIGHT]->renderCubes();
            }
        }
    }
}





void World::swapBuffers()
{
    GX_DrawDone();

    frame ^= 1;
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(framebuffer[frame],GX_TRUE);

    VIDEO_SetNextFramebuffer(framebuffer[frame]);

    VIDEO_Flush();

    VIDEO_WaitVSync();

    if (screenMode->viTVMode & VI_NON_INTERLACE) {
        VIDEO_WaitVSync();
    }
}


void World::initGX()
{
    screenMode = NULL;
    frame = 0;
    fifoBuffer = NULL;
    frameCount = 0;
    backgroundColor = {0xFF,0xFF,0xFF,0xFF};

    VIDEO_Init();

    screenMode = VIDEO_GetPreferredMode(NULL);

    framebuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));
    framebuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));

    VIDEO_Configure(screenMode);
    VIDEO_SetNextFramebuffer(framebuffer[frame]);
    VIDEO_SetPostRetraceCallback(copy_buffers);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(screenMode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    fifoBuffer = (u8*)MEM_K0_TO_K1(memalign(32, FIFO_SIZE));
    memset((void*)fifoBuffer, 0x0, FIFO_SIZE);


    GX_Init(fifoBuffer, FIFO_SIZE);
    GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    GX_SetCopyClear(backgroundColor, 0x00FFFFFF);

    GX_SetViewport(0, 0, screenMode->fbWidth, screenMode->efbHeight, 0.0f, 1.0f);
	float yscale = GX_GetYScaleFactor(screenMode->efbHeight, screenMode->xfbHeight);
	int xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopySrc(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopyDst(screenMode->fbWidth, xfbHeight);
	GX_SetCopyFilter(screenMode->aa, screenMode->sample_pattern, GX_TRUE, screenMode->vfilter);
	GX_SetFieldMode(screenMode->field_rendering, ((screenMode->viHeight==2*screenMode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

    GX_SetCullMode(GX_CULL_NONE);
    GX_CopyDisp(framebuffer[frame], GX_TRUE);
    GX_SetDispCopyGamma(GX_GM_1_0);


    GX_InvVtxCache();
	GX_ClearVtxDesc();

    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_NONE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX8);

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    //GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

    GX_SetNumChans(1);
    GX_SetNumTexGens(1);
    GX_SetNumTevStages(1);

    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    GX_InvalidateTexAll();


    guPerspective(projection, 90, 1.333f, 0.1f, 1000.0f);
    GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);

    console_init(framebuffer[frame],20,20,screenMode->fbWidth,screenMode->xfbHeight,screenMode->fbWidth*VI_DISPLAY_PIX_SZ);
    printf("\x1b[%d;%dH\n", 2, 0);
}

void World::exitGX()
{
    GX_DrawDone();
    GX_AbortFrame();

    VIDEO_ClearFrameBuffer(screenMode, framebuffer[0], 0xFFFFFFFF);
    VIDEO_ClearFrameBuffer(screenMode, framebuffer[1], 0xFFFFFFFF);

    if (framebuffer[0]  != NULL) {  free(MEM_K1_TO_K0(framebuffer[0]));  framebuffer[0]  = NULL;  }
    if (framebuffer[1]  != NULL) {  free(MEM_K1_TO_K0(framebuffer[1]));  framebuffer[1]  = NULL;  }
    if (fifoBuffer != NULL)      {  free(fifoBuffer);                    fifoBuffer      = NULL;  }
}

void World::copy_buffers(u32 count)
{

        //frameCount++;

}

