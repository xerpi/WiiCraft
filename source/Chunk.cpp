#include "Chunk.h"


Chunk::Chunk(int _indexX, int _indexY, int _indexZ)
{
   indexX = _indexX;
   indexY = _indexY;
   indexZ = _indexZ;

   int x, y, z;
    for(z = 0; z < CHUNK_SIZE; z++) {
        for(y = 0; y < CHUNK_SIZE; y++) {
            for(x = 0; x < CHUNK_SIZE; x++) {
                int ID;
                if(indexY < 4) {
                    ID = BLOCK_AIR;
                } else {
                    ID = rand() % 2 + 1;
                }
                blockArray[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] = new Block(ID);
            }
        }
    }
    dispList = memalign(32, DISPLIST_SIZE);
}


Chunk::~Chunk()
{
    int x, y, z;
    for(z = 0; z < CHUNK_SIZE; z++) {
        for(y = 0; y < CHUNK_SIZE; y++) {
            for(x = 0; x < CHUNK_SIZE; x++) {
                delete blockArray[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
            }
        }
    }
    free(dispList);
}

void Chunk::setNeighbours(Chunk *_topC, Chunk *_bottomC, Chunk *_rightC, Chunk *_leftC, Chunk *_frontC, Chunk *_backC)
{
   topC    = _topC;
   bottomC = _bottomC;
   rightC  = _rightC;
   leftC   = _leftC;
   frontC  = _frontC;
   backC   = _backC;
}


Block *Chunk::getBlockAt(int x, int y, int z)
{
   if( (x < 0 || x >= CHUNK_SIZE) || (y < 0 || y >= CHUNK_SIZE) || (z < 0 || z >= CHUNK_SIZE)) {
      return NULL;
   }
   return blockArray[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
}

/*
struct cubeFaceData_t {
    struct {
        u8 x, y, z;
    }cubePosIndex;
    u8 cubeID;
    u8 face;
};

*/

void Chunk::rebuildCubeRenderList()
{
    GX_BeginDispList(dispList, DISPLIST_SIZE);
    GX_SetArray(GX_VA_TEX0, blockTexCoordVertices, 2 * sizeof(float));
    Chunk *p;
    Block *b;
    float pos_x, pos_y, pos_z = indexZ * CHUNK_SIZE;
    int x, y, z;
    for(z = 0; z < CHUNK_SIZE; z++) {
        pos_y = indexY * CHUNK_SIZE;
        for(y = 0; y < CHUNK_SIZE; y++) {
            pos_x = indexX * CHUNK_SIZE;
            for(x = 0; x < CHUNK_SIZE; x++) {
                b = getBlockAt(x, y, z);
                if(b->ID != BLOCK_AIR) {
                    //Top block neighbour
                    if(y < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x, y + 1, z);
                        if(b->hasTransparency()) {
                            TextureManager.setBlockTextureTop(b->ID);
                            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(0);
                                GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z);
                                    GX_TexCoord1x8(1);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(2);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                    GX_TexCoord1x8(3);
                                GX_End();
                                }
                    }else {
                        p = topC;
                        if(p) {
                            b = p->getBlockAt(x, 0, z);
                            if(b->hasTransparency()) {
                               TextureManager.setBlockTextureTop(b->ID);
                               GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(0);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(1);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                     GX_TexCoord1x8(2);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                     GX_TexCoord1x8(3);
                               GX_End();
                            }
                        }
                    }
                    //Bottom block neighbour
                    if(y > 0) {
                        b = getBlockAt(x, y - 1, z);
                        if(b->hasTransparency()) {
                            TextureManager.setBlockTextureBottom(b->ID);
                            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                GX_Position3f32(pos_x, pos_y, pos_z);
                                    GX_TexCoord1x8(0);
                                GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(1);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                    GX_TexCoord1x8(2);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(3);
                            GX_End();
                        }
                    }else {
                        p = bottomC;
                        if(p) {
                            b = p->getBlockAt(x, (CHUNK_SIZE-1), z);
                            if(b->hasTransparency()) {
                                TextureManager.setBlockTextureBottom(b->ID);
                                GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                    GX_Position3f32(pos_x, pos_y, pos_z);
                                        GX_TexCoord1x8(0);
                                    GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(1);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                        GX_TexCoord1x8(2);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(3);
                                GX_End();
                            }
                        }
                    }
                    TextureManager.setBlockTextureSide(b->ID);
                    //Left block neighbour
                    if(x > 0) {
                        b = getBlockAt(x - 1, y, z);
                        if(b->hasTransparency()) {
                            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                GX_Position3f32(pos_x, pos_y, pos_z);
                                    GX_TexCoord1x8(0);
                                GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z);
                                    GX_TexCoord1x8(1);
                                GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(2);
                                GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(3);
                            GX_End();
                        }
                    }else {
                        p = leftC;
                        if(p) {
                            b = p->getBlockAt((CHUNK_SIZE-1), y, z);
                            if(b->hasTransparency()) {
                                GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                    GX_Position3f32(pos_x, pos_y, pos_z);
                                        GX_TexCoord1x8(0);
                                    GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z);
                                        GX_TexCoord1x8(1);
                                    GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(2);
                                    GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(3);
                                GX_End();
                            }
                        }
                    }
                    //Right block neighbour
                    if(x < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x + 1, y, z);
                        if(b->hasTransparency()) {
                            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                    GX_TexCoord1x8(0);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                    GX_TexCoord1x8(1);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(2);
                                GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                    GX_TexCoord1x8(3);
                            GX_End();
                        }
                    }else {
                        p = rightC;
                        if(p) {
                            b = p->getBlockAt(0, y, z);
                            if(b->hasTransparency()) {
                                GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                        GX_TexCoord1x8(0);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                        GX_TexCoord1x8(1);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(2);
                                    GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                        GX_TexCoord1x8(3);
                                GX_End();
                            }
                        }
                    }

                    //Front block neighbour
                    if(z < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x, y, z + 1);
                        if(b->hasTransparency()) {
                           GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                              GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                 GX_TexCoord1x8(0);
                              GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                 GX_TexCoord1x8(1);
                              GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                 GX_TexCoord1x8(2);
                              GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                 GX_TexCoord1x8(3);
                           GX_End();
                        }
                    }else {
                        p = frontC;
                        if(p) {
                            b = p->getBlockAt(x, y, 0);
                            if(b->hasTransparency()) {
                               GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                  GX_Position3f32(pos_x, pos_y, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(0);
                                  GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(1);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(2);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z + BLOCK_SIZE);
                                     GX_TexCoord1x8(3);
                               GX_End();
                            }
                        }
                    }
                    //Back block neighbour
                    if(z > 0) {
                        b = getBlockAt(x, y, z - 1);
                        if(b->hasTransparency()) {
                           GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                              GX_Position3f32(pos_x, pos_y, pos_z);
                                 GX_TexCoord1x8(0);
                              GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z);
                                 GX_TexCoord1x8(1);
                              GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                 GX_TexCoord1x8(2);
                              GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                 GX_TexCoord1x8(3);
                           GX_End();
                        }
                    }else {
                        p = backC;
                        if(p) {
                            b = p->getBlockAt(x, y, (CHUNK_SIZE-1));
                            if(b->hasTransparency()) {
                               GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                                  GX_Position3f32(pos_x, pos_y, pos_z);
                                     GX_TexCoord1x8(0);
                                  GX_Position3f32(pos_x, pos_y + BLOCK_SIZE, pos_z);
                                     GX_TexCoord1x8(1);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y, pos_z);
                                     GX_TexCoord1x8(2);
                                  GX_Position3f32(pos_x + BLOCK_SIZE, pos_y + BLOCK_SIZE, pos_z);
                                     GX_TexCoord1x8(3);
                               GX_End();
                            }
                        }
                    }
                }
                pos_x += BLOCK_SIZE;
            }
            pos_y += BLOCK_SIZE;
        }
        pos_z += BLOCK_SIZE;
    }
    dispListSize = GX_EndDispList();
}


void Chunk::updateNeighbours()
{
   if(topC)    topC->rebuildCubeRenderList();
   if(bottomC) bottomC->rebuildCubeRenderList();
   if(rightC)  rightC->rebuildCubeRenderList();
   if(leftC)   leftC->rebuildCubeRenderList();
   if(frontC)  frontC->rebuildCubeRenderList();
   if(backC )  backC ->rebuildCubeRenderList();
}


void Chunk::renderCubes()
{
    GX_CallDispList(dispList, dispListSize);
}





