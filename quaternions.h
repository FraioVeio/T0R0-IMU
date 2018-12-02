#ifndef QUATERNIONS_h
#define QUATERNIONS_h

#include <stdlib.h>
#include <math.h>

float *quaternionMult(float *q, float *r);
void quaternionInv(float *q);
float *quaternionZero();
float quaternionModule(float *q);


#endif