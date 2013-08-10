#include "Image.h"


//Destructor
	Image::~Image()
	{
		clear();
	}

//Methods

	//Clears the image
		void Image::clear()
		{
            if(data) {
                free(data);
                data = NULL;
            }
		}

	//Loads from file
		int Image::loadFromFile(char *filename)
		{
		    if(data) clear();
			int ret;
			PNGUPROP imgProp;
			IMGCTX ctx = PNGU_SelectImageFromDevice(filename);
			ret = PNGU_GetImageProperties(ctx, &imgProp);
			if(ret != PNGU_OK) return -1;

			int len = imgProp.imgWidth * imgProp.imgHeight * 4;
			//if(len%32) len += (32-len%32);

			data = (uint8_t*)memalign(32, len);

			ret = PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, data, 0xFF);
			if(ret == PNGU_OK) {
				width = imgProp.imgWidth;
				height = imgProp.imgHeight;
			} else {
				free(data);
				return -2;
			}
			PNGU_ReleaseImageContext(ctx);
			DCFlushRange(data, len);

			GX_InitTexObj(&texObj, // objeto textura a inicializar
						 (void *)data, // direccion de memoria de la textura
						 width,// ancho
						 height,// alto
						 GX_TF_RGBA8, // formato de color
						 GX_CLAMP, // Wrap Mode para S
						 GX_CLAMP, // Wrap Mode para T
						 GX_FALSE // No MipMap
			);
			GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
			return 1;
		}

	//Loads from buffer
		int Image::loadFromBuffer(uint8_t *buf_data)
		{
		    if(data) clear();
			int ret;
			PNGUPROP imgProp;
			IMGCTX ctx = PNGU_SelectImageFromBuffer(buf_data);
			ret = PNGU_GetImageProperties(ctx, &imgProp);
			if(ret != PNGU_OK)
				return -1;

			int len = imgProp.imgWidth * imgProp.imgHeight * 4;
			if(len%32) len += (32-len%32);

			data = (uint8_t*)memalign(32, len);

			ret = PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, data, 0xFF);
			if(ret == PNGU_OK) {
				width = imgProp.imgWidth;
				height = imgProp.imgHeight;
				DCFlushRange(data, len);
			}else {
				free(data);
				return -2;
			}
			PNGU_ReleaseImageContext(ctx);

			GX_InitTexObj(&texObj, // objeto textura a inicializar
						 (void *)data, // direccion de memoria de la textura
						 width,// ancho
						 height,// alto
						 GX_TF_RGBA8, // formato de color
						 GX_CLAMP, // Wrap Mode para S
						 GX_CLAMP, // Wrap Mode para T
						 GX_FALSE // No MipMap
			);

			GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
			return 1;
		}


