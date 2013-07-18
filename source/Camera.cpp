#include "Camera.h"


//Constructor
	Camera::Camera(guVector *positionP, float *pitchP, float *yawP)
	{
		position = positionP;
		pitch = pitchP;
		yaw = yawP;
	}


//Methods
	void Camera::updateMatrix()
	{
		guMtxIdentity(cameraMatrix);

		matrix_rotate_x(cameraMatrix, *pitch);
		matrix_rotate_y(cameraMatrix, *yaw);

		matrix_translate(cameraMatrix, -position->x, -position->y, -position->z);

	}

	guVector Camera::getLookVector()
	{
		float *p = (float *)cameraMatrix[2];
		guVector v = {*(p), *(p+1), *(p+2)};
		return v;
	}

	guVector Camera::getForwardVector()
	{
		return cross_product(getRightVector(), UP_VECTOR);
	}

	guVector Camera::getRightVector()
	{
		float *p = (float *)cameraMatrix;
		guVector v = {*(p), *(p+1), *(p+2)};
		return v;
	}

	guVector Camera::getUpVector()
	{
		float *p = (float *)cameraMatrix[1];
		guVector v = {*(p), *(p+1), *(p+2)};
		return v;
	}

/*	void Camera::move(float distance, guVector direction)
	{
		position->x += direction.x * distance;
		position->y += direction.y * distance;
		position->z += direction.z * distance;
	}

	void Camera::translate(float x, float y, float z)
	{
		position->x = x;
		position->y = y;
		position->z = z;
	}

*/
