#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <gccore.h>
#include <math.h>
#include <string.h>
#include "Math_utils.h"


class Camera
{
public:
//Constructor
	Camera(guVector *positionP, float *pitchP, float *yawP);

//Methods
	guVector getLookVector();
	guVector getUpVector();
	guVector getForwardVector();
	guVector getRightVector();
	void updateMatrix();
	//void move(float distance, guVector direction);
	//void translate(float x, float y, float z);
//private:
	guVector *position;
	float *pitch, *yaw;

	Mtx cameraMatrix;
};



#endif
