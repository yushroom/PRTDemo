#ifndef _SHROTATION_H
#define _SHROTATION_H
#include "global.h"

void RotateSHCoefficients(	int numBands, float * unrotatedCoeffs, float * rotatedCoeffs, float theta, float phi);

//Get a matrix for rotating one band of coefficients about the Z axis
void GetZRotationMatrix(int band, float * entries, float angle);

//Get a matrix for rotating one band of coefficients about the X axis, by 90 degrees
void GetX90DegreeRotationMatrix(int band, float * entries);

//Apply a matrix to a vector of coefficients
void ApplyMatrix(int size, float * matrix, bool transpose, float * inVector, float * outVector);
#endif

