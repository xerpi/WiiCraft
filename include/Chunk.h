#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <gccore.h>
#include "Block.h"
#include "TextureManager.h"
#include "MatrixStack.h"

enum cubeFace_t {
    CUBE_FACE_FRONT  = 0b1,
    CUBE_FACE_RIGHT  = 0b10,
    CUBE_FACE_BACK   = 0b100,
    CUBE_FACE_LEFT   = 0b1000,
    CUBE_FACE_TOP    = 0b10000,
    CUBE_FACE_BOTTOM = 0b100000
};

struct cubeFaceData_t {
    struct {
        u8 x, y, z;
    }cubePosIndex;
    u8 cubeID;
    u8 face;
};

const int CHUNK_SIZE = 12;
const int BLOCK_ARRAY_SIZE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
class Chunk
{
public:
    Chunk(int _indexX, int _indexY, int _indexZ);

    ~Chunk();

    void setNeighbours(Chunk *_topC, Chunk *_bottomC, Chunk *_rightC, Chunk *_leftC, Chunk *_frontC, Chunk *_backC);
    Block *getBlockAt(int x, int y, int z);

    void updateNeighbours();
    void rebuildCubeRenderList();
    void renderCubes();
    void addFace(u8 x, u8 y, u8 z, u8 ID, u8 face) {
        cubeFaceDataList.push_back({{x, y, z}, ID, face});
    }

    Chunk *topC, *bottomC, *rightC, *leftC, *frontC, *backC;
    int indexX, indexY, indexZ;

    //private:
    Block *blockArray[BLOCK_ARRAY_SIZE];
    std::vector <struct cubeFaceData_t> cubeFaceDataList;

};

#endif
