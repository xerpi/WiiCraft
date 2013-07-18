#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "Image.h"

const float BLOCK_SIZE = 1.0f;
const float BLOCK_HALF_SIZE = BLOCK_SIZE/2.0f;



class Block
{
public:
    Block(u8 _ID_) : ID(_ID_) {}
    bool hasTransparency();

    u8 ID;
};

#define BLOCK_AIR   0
#define BLOCK_GRASS 1
#define BLOCK_STONE 2

#endif


