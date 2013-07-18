#ifndef _WORLD_H_
#define _WORLD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <gccore.h>
#include <FreeTypeGX.h>

#include "Image.h"
#include "Chunk.h"
#include "Block.h"
#include "Camera.h"
#include "TextureManager.h"


#define FIFO_SIZE (4 * 1024 * 1024)

const int WORLD_SIZE = 12;
const int CHUNK_ARRAY_SIZE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class World
{
public:
    World();
    ~World();

    void swapBuffers();
    void drawChunks();
    Chunk *getChunkAt(int x, int y, int z);


//Testing
    Camera *camera;
    guVector position;
    float pitch, yaw;
//private:

    Chunk *chunkArray[CHUNK_ARRAY_SIZE];

    void initGX();
    void exitGX();
    static void copy_buffers(u32 count);
    void initChunkArray();
    void deleteChunkArray();

//Variables


    //GX
    GXRModeObj *screenMode;
    void *framebuffer[2];
    int frame;
    void *fifoBuffer;
    int frameCount;


    GXColor backgroundColor;
    Mtx44 projection;
    Mtx model, world, cameraMatrix, modelView;


};
#endif
