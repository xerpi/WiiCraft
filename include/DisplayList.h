#ifndef _DISPLAYLIST_H_
#define _DISPLAYLIST_H_

#include <vector>
#include <stdlib.h>
#include <gccore.h>
#include "Image.h"

#include "grass_png.h"

typedef struct _vec2f32 {
    f32 x, y;
}Vector2f32;


enum DispListCmdID {
    DISPLISTCMD_POSITION,
    DISPLISTCMD_TEXCOORD,
    DISPLISTCMD_IMAGE
};


class DisplayList
{
public:
    DisplayList(void) {

    }

    ~DisplayList() {
        clear();
    }

    void render() {

        std::vector <guVector>::iterator   posIt = positionList.begin();
        if(positionList.size() == 0) return;
        std::vector <Vector2f32>::iterator texIt = texCoordList.begin();
        std::vector <Image *>::iterator    imgIt = imageList.begin();
        std::vector <int>::iterator        cmdIt = commandList.begin();

        GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, positionList.size());

        for(; cmdIt != commandList.end(); ++cmdIt) {
            switch(*cmdIt)
            {
            case DISPLISTCMD_POSITION:
                GX_Position3f32(posIt->x, posIt->y, posIt->z); ++posIt;
                break;
            case DISPLISTCMD_TEXCOORD:
                GX_TexCoord2f32(texIt->x, texIt->y); ++texIt;
                break;
            case DISPLISTCMD_IMAGE:
                (*imgIt)->setGX(GX_VTXFMT0); ++imgIt;
                break;
            default:
                break;
            }
        }
        GX_End();
        GX_Flush();
    }

    void clear() {
        positionList.clear();
        texCoordList.clear();
        imageList.clear();
        commandList.clear();
    }

    void addPosition(guVector &v) {
        positionList.push_back(v);
        commandList.push_back(DISPLISTCMD_POSITION);
    }

    void addPosition(float x, float y, float z) {
        positionList.push_back({x, y, z});
        commandList.push_back(DISPLISTCMD_POSITION);
    }

    void addTexCoord(Vector2f32 &v) {
        texCoordList.push_back(v);
        commandList.push_back(DISPLISTCMD_TEXCOORD);
    }

    void addTexCoord(float s, float t) {
        texCoordList.push_back({s, t});
        commandList.push_back(DISPLISTCMD_TEXCOORD);
    }

    void addImage(Image *p) {
        imageList.push_back(p);
        commandList.push_back(DISPLISTCMD_IMAGE);
    }


private:
    std::vector <guVector>    positionList;
    std::vector <Vector2f32>  texCoordList;
    std::vector <Image *>     imageList;
    std::vector <int>         commandList;

};

#endif // _DISPLAYLIST_H_
