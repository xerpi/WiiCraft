#include "Block.h"


bool Block::hasTransparency()
{
    switch(ID)
    {
    case BLOCK_AIR:
        return 1;
    default:
       return 0;
    }
}
