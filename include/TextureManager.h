#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <vector>
#include "Image.h"
#include "Block.h"


extern float blockPositionVertices[24] ATTRIBUTE_ALIGN(32);
extern float blockTexCoordVertices[8] ATTRIBUTE_ALIGN(32);


//3 textures per block, TOP, BOTTOM and SIDE
struct Block_texture_data_t{
    Image *top, *bottom, *side;
    Block_texture_data_t(Image *_top, Image *_bottom, Image *_side) : top(_top), bottom(_bottom), side(_side) {}
};

class TextureManager_t
{
public:
   TextureManager_t();
   ~TextureManager_t();
   void loadBlockTextureData();

   void setBlockTextureTop(u8 id);
   void setBlockTextureBottom(u8 id);
   void setBlockTextureSide(u8 id);

//private:
    std::vector<struct Block_texture_data_t *> blockTextures;
    std::vector<Image *> textureArray;
};

extern TextureManager_t TextureManager;

#endif



/* ID's

0: Air
1: Grass
2: Stone


*/
