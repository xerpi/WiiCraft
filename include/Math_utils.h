#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#include <math.h>
#include <gccore.h>

extern guVector UP_VECTOR;
extern guVector DOWN_VECTOR;


//Converts spherical(phi, theta, rho) coordinates to cartesian coordinates (x,y,z)
guVector spherical_to_cartesian(float phi, float theta, float rho);
////Converts spherical(phi, theta, rho) coordinates to cartesian coordinates (x,y,z) normalized
guVector spherical_to_cartesian_norm(float phi, float theta);
//Cross product between a and b
guVector cross_product(guVector a, guVector b);
//Dot product between a and b
float dot_product(guVector a, guVector b);
//Vector length/magnitude
float vector_length(guVector v);
//Normalize vector
guVector vector_normalize(guVector v);
//Get angle (theta) between a and b
float vector_theta(guVector a, guVector b);
//Zeroes a vector
void vector_zero(guVector *v);

//Set matrix rotation X
void matrix_set_rotation_x(MtxP m, float angle);
//Set matrix rotation Y
void matrix_set_rotation_y(MtxP m, float angle);
//Set matrix rotation Z
void matrix_set_rotation_z(MtxP m, float angle);

//Translate matrix (x,y,z)
void matrix_translate(MtxP m, float x, float y, float z);

//Matrix 3x3 identity
void matrix_3x3_identity(MtxP m);
//Matrix 4x4 identity
void matrix_4x4_identity(Mtx44 m);

//Rotate matrix around X axis
void matrix_rotate_x(MtxP m, float angle);
//Rotate matrix around Y axis
void matrix_rotate_y(MtxP m, float angle);
//Rotate matrix around Z axis
void matrix_rotate_z(MtxP m, float angle);

//Multply matrix (if m1 and dest should be different!!)
void matrix_mult(MtxP m1, MtxP m2, MtxP dest);
//Multply 3x3 matrix (if m1 and dest should be different!!)
void matrix_3x3_mult(MtxP m1, MtxP m2, MtxP dest);
//Multply 4x4 matrix (if m1 and dest should be different!!)
void matrix_4x4_mult(Mtx44 m1, Mtx44 m2, Mtx44 dest);



#endif // _MATH_H_
