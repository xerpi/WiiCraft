#include "TextureManager.h"

#include "grass_png.h"
#include "stone_png.h"

TextureManager_t TextureManager;


TextureManager_t::TextureManager_t()
{
    loadBlockTextureData();
}


TextureManager_t::~TextureManager_t()
{
    textureArray.clear();
    blockTextures.clear();
}

void TextureManager_t::loadBlockTextureData()
{
    textureArray.push_back(new Image((u8*)grass_png));
    textureArray.push_back(new Image((u8*)stone_png));

    blockTextures.push_back(new struct Block_texture_data_t(textureArray[0], textureArray[0], textureArray[0])); //grass
    blockTextures.push_back(new struct Block_texture_data_t(textureArray[1], textureArray[1], textureArray[1])); //stone
}


void TextureManager_t::setBlockTextureTop(u8 id)
{
    if(id < 1 || id > blockTextures.size()) return;
    blockTextures[id -1]->top->setGX(GX_VTXFMT0);
}

void TextureManager_t::setBlockTextureBottom(u8 id)
{
    if(id < 1 || id > blockTextures.size()) return;
    blockTextures[id -1]->bottom->setGX(GX_VTXFMT0);
}

void TextureManager_t::setBlockTextureSide(u8 id)
{
    if(id < 1 || id > blockTextures.size()) return;
    blockTextures[id -1 ]->side->setGX(GX_VTXFMT0);
}


float blockPositionVertices[24] ATTRIBUTE_ALIGN(32) = {
    0.0f,       0.0f,       BLOCK_SIZE,
    0.0f,       BLOCK_SIZE, BLOCK_SIZE,
    BLOCK_SIZE, 0.0f,       BLOCK_SIZE,
    BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
    BLOCK_SIZE, 0.0f,       0.0f,
    BLOCK_SIZE, BLOCK_SIZE, 0.0f,
    0.0f,       0.0f,       0.0f,
    0.0f,       BLOCK_SIZE, 0.0f
};


float blockTexCoordVertices[8] ATTRIBUTE_ALIGN(32) = {
       0.0f, 1.0f,
       0.0f, 0.0f,
       1.0f, 1.0f,
       1.0f, 0.0f,
};
