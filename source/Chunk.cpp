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
                if(indexY < 3) {
                    ID = BLOCK_GRASS;
                } else if(indexY >= 3 && indexY < 6){
                    ID = BLOCK_STONE;
                } else {
                    ID = BLOCK_AIR;
                }
                blockArray[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] = new Block(ID);
            }
        }
    }
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
    cubeFaceDataList.clear();
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
    cubeFaceDataList.clear();
    Chunk *p;
    Block *b;
    u8 x, y, z;
    for(z = 0; z < CHUNK_SIZE; z++) {
        for(y = 0; y < CHUNK_SIZE; y++) {
            for(x = 0; x < CHUNK_SIZE; x++) {
                b = getBlockAt(x, y, z);
                if(b->ID != BLOCK_AIR) {
                    //Top block neighbour
                    if(y < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x, y + 1, z);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_TOP);
                        }
                    }else {
                        p = topC;
                        if(p) {
                            b = p->getBlockAt(x, 0, z);
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_TOP);
                            }
                        }
                    }
                    //Bottom block neighbour
                    if(y > 0) {
                        b = getBlockAt(x, y - 1, z);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_BOTTOM);
                        }
                    }else {
                        p = bottomC;
                        if(p) {
                            b = p->getBlockAt(x, (CHUNK_SIZE-1), z);
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_BOTTOM);
                            }
                        }
                    }

                    //Left block neighbour
                    if(x > 0) {
                        b = getBlockAt(x - 1, y, z);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_LEFT);
                        }
                    }else {
                        p = leftC;
                        if(p) {
                            b = p->getBlockAt((CHUNK_SIZE-1), y, z);
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_LEFT);
                            }
                        }
                    }
                    //Right block neighbour
                    if(x < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x + 1, y, z);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_RIGHT);
                        }
                    }else {
                        p = rightC;
                        if(p) {
                            b = p->getBlockAt(0, y, z);
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_RIGHT);
                            }
                        }
                    }
                    //Front block neighbour
                    if(z < (CHUNK_SIZE-1)) {
                        b = getBlockAt(x, y, z + 1);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_FRONT);
                        }
                    }else {
                        p = frontC;
                        if(p) {
                            b = p->getBlockAt(x, y, 0);
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_FRONT);
                            }
                        }
                    }
                    //Back block neighbour
                    if(z > 0) {
                        b = getBlockAt(x, y, z - 1);
                        if(b->hasTransparency()) {
                            addFace(x, y, z, b->ID, CUBE_FACE_BACK);
                        }
                    }else {
                        p = backC;
                        if(p) {
                            b = p->getBlockAt(x, y, (CHUNK_SIZE-1));
                            if(b->hasTransparency()) {
                                addFace(x, y, z, b->ID, CUBE_FACE_BACK);
                            }
                        }
                    }
                }
            }
        }
    }
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
    GX_SetArray(GX_VA_POS, blockPositionVertices, 3 * sizeof(float));
    GX_SetArray(GX_VA_TEX0, blockTexCoordVertices, 2 * sizeof(float));

    GX_SetCurrentMtx(GX_PNMTX0);

    float x, y, z;
    std::vector <struct cubeFaceData_t>::iterator it;
    for(it = cubeFaceDataList.begin(); it != cubeFaceDataList.end(); ++it) {
        x = indexX * CHUNK_SIZE + it->cubePosIndex.x * BLOCK_SIZE;
        y = indexY * CHUNK_SIZE + it->cubePosIndex.y * BLOCK_SIZE;
        z = indexZ * CHUNK_SIZE + it->cubePosIndex.z * BLOCK_SIZE;

        MatrixStack.Push();
        MatrixStack.Translate(x, y, z);
        MatrixStack.Set(GX_PNMTX0);

        if(it->face & CUBE_FACE_FRONT) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(0);
                    GX_TexCoord1x8(0);
                GX_Position1x8(1);
                    GX_TexCoord1x8(1);
                GX_Position1x8(2);
                    GX_TexCoord1x8(2);
                GX_Position1x8(3);
                    GX_TexCoord1x8(3);
            GX_End();
        }
        if(it->face & CUBE_FACE_RIGHT) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(2);
                    GX_TexCoord1x8(0);
                GX_Position1x8(3);
                    GX_TexCoord1x8(1);
                GX_Position1x8(4);
                    GX_TexCoord1x8(2);
                GX_Position1x8(5);
                    GX_TexCoord1x8(3);
            GX_End();
        }
        if(it->face & CUBE_FACE_BACK) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(4);
                    GX_TexCoord1x8(0);
                GX_Position1x8(5);
                    GX_TexCoord1x8(1);
                GX_Position1x8(6);
                    GX_TexCoord1x8(2);
                GX_Position1x8(7);
                    GX_TexCoord1x8(3);
            GX_End();
        }
        if(it->face & CUBE_FACE_LEFT) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(6);
                    GX_TexCoord1x8(0);
                GX_Position1x8(7);
                    GX_TexCoord1x8(1);
                GX_Position1x8(0);
                    GX_TexCoord1x8(2);
                GX_Position1x8(1);
                    GX_TexCoord1x8(3);
            GX_End();
        }
        if(it->face & CUBE_FACE_TOP) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(1);
                    GX_TexCoord1x8(0);
                GX_Position1x8(7);
                    GX_TexCoord1x8(1);
                GX_Position1x8(3);
                    GX_TexCoord1x8(2);
                GX_Position1x8(5);
                    GX_TexCoord1x8(3);
            GX_End();
        }
        if(it->face & CUBE_FACE_BOTTOM) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
                GX_Position1x8(6);
                    GX_TexCoord1x8(0);
                GX_Position1x8(0);
                    GX_TexCoord1x8(1);
                GX_Position1x8(4);
                    GX_TexCoord1x8(2);
                GX_Position1x8(2);
                    GX_TexCoord1x8(3);
            GX_End();
        }

        MatrixStack.Pop();
    }

}





