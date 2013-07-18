#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <debug.h>
#include "World.h"
#include "Chunk.h"
#include "Math_utils.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Image.hpp"

#include "stone_png.h"

MatrixStack_t MatrixStack;

WPADData *wiimoteData;
joystick_t *nunchukJoystick;
u32 wiimoteExpansion;
float nunchuk_x = 0.0f, nunchuk_y = 0.0f;
u32 pressed = 0, old_pressed = 0, held = 0;

World world;

void read_controls();
void move_camera();

int main(void) {

    WPAD_Init();

    float chunk_length = BLOCK_SIZE * CHUNK_SIZE;
    float world_length = chunk_length * WORLD_SIZE;

    int chunkX, chunkY, chunkZ;
    int posX, posY, posZ;
    Chunk *c;
    Block *p;


    while (1)
    {
        printf("\x1b[%d;%dH\n", 2, 0);
        read_controls();

        move_camera();

        world.camera->updateMatrix();

        if((world.position.x > 0) && (world.position.y > 0) && (world.position.z > 0) &&
            (world.position.x < world_length) && (world.position.y < world_length) &&
            (world.position.z < world_length)) {

                chunkX = (int)(world.position.x/chunk_length);
                chunkY = (int)(world.position.y/chunk_length);
                chunkZ = (int)(world.position.z/chunk_length);

                posX = (int)((world.position.x-(chunkX*chunk_length))/BLOCK_SIZE);
                posY = (int)((world.position.y-(chunkY*chunk_length))/BLOCK_SIZE);
                posZ = (int)((world.position.z-(chunkZ*chunk_length))/BLOCK_SIZE);

                c = world.getChunkAt(chunkX, chunkY, chunkZ);
                p = c->getBlockAt(posX, posY, posZ);

                printf("chunkX: %i  chunkY: %i  chunkZ: %i\n", chunkX, chunkY, chunkZ);
                printf("posX: %i  posY: %i  pos: %i\n", posX, posY, posZ);
                printf("ID:%i\n", p->ID);

                if(held & WPAD_BUTTON_A) {
                    p->ID = BLOCK_AIR;
                    c->rebuildCubeRenderList();
                    c->updateNeighbours();
                } else if(held & WPAD_BUTTON_B) {
                    p->ID = BLOCK_STONE;
                    c->rebuildCubeRenderList();
                    c->updateNeighbours();
                } else if(held & WPAD_BUTTON_1) {
                    p->ID = BLOCK_GRASS;
                    c->rebuildCubeRenderList();
                    c->updateNeighbours();
                }
        }

        MatrixStack.baseMatrix(world.camera->cameraMatrix);

        TextureManager.setBlockTextureSide(1);

        MatrixStack.Push();
        world.drawChunks();
        MatrixStack.Pop();

        MatrixStack.Set(GX_PNMTX0);

        GX_SetArray(GX_VA_POS, blockPositionVertices, 3 * sizeof(float));
        GX_SetArray(GX_VA_TEX0, blockTexCoordVertices, 2 * sizeof(float));

        //0, 0, 0
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


        world.swapBuffers();

        if (pressed & WPAD_BUTTON_HOME) break;
        old_pressed = pressed;
    }
    return 0;
}

void move_camera()
{
    if(held & WPAD_BUTTON_2) {
        world.position = {0.0f, 0.0f, 0.0f};
        world.pitch = 0.0f;
        world.yaw = 3.14f;
    }
   if(wiimoteExpansion == WPAD_EXP_NUNCHUK)
		{
			if(wiimoteData->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			{

			}
			if(nunchukJoystick->mag >= 0.2f)
			{
			   #define MOV_SPEED 5.0f

				float n_angle = DegToRad(nunchukJoystick->ang - 90);

				float c = cos(n_angle)*nunchukJoystick->mag*MOV_SPEED / 7.5f;
				float s = sin(n_angle)*nunchukJoystick->mag*MOV_SPEED / 7.5f;
				guVector right = world.camera->getRightVector();
				guVector forw = world.camera->getLookVector();

            world.position.x += (right.x * c);
            world.position.y += (right.y * c);
            world.position.z += (right.z * c);

            world.position.x += (forw.x * s);
            world.position.y += (forw.y * s);
            world.position.z += (forw.z * s);
			}
		}

        if(held & WPAD_NUNCHUK_BUTTON_C) {
            world.position.y += 0.5f;
        }
        if(held & WPAD_NUNCHUK_BUTTON_Z) {
            world.position.y -= 0.5f;
        }

      #define ROT_SPEED 0.05f
		if(held & WPAD_BUTTON_UP)
			world.pitch += ROT_SPEED;
		if(held & WPAD_BUTTON_DOWN)
			world.pitch -= ROT_SPEED;

		if(held & WPAD_BUTTON_RIGHT)
			world.yaw -= ROT_SPEED;
		if(held & WPAD_BUTTON_LEFT)
			world.yaw += ROT_SPEED;

}


void read_controls()
{
    WPAD_ScanPads();
    WPAD_Probe(0, &wiimoteExpansion);
    wiimoteData = WPAD_Data(0);
    nunchukJoystick = &wiimoteData->exp.nunchuk.js;
    held = wiimoteData->btns_h;
    pressed = wiimoteData->btns_d;
    if(wiimoteExpansion == WPAD_EXP_NUNCHUK) {
        float n_angle = DegToRad(nunchukJoystick->ang);
        nunchuk_x =  cosf(n_angle) * nunchukJoystick->mag;
        nunchuk_y =  sinf(n_angle) * nunchukJoystick->mag;
    }
}

