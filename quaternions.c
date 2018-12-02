#include "quaternions.h"

float *quaternionMult(float *q, float *r) {
    float *t = (float*) calloc(4, sizeof(float));

    t[0] = r[0]*q[0] - r[1]*q[1] - r[2]*q[2] - r[3]*q[3];
    t[1] = r[0]*q[1] + r[1]*q[0] - r[2]*q[3] + r[3]*q[2];
    t[2] = r[0]*q[2] + r[1]*q[3] + r[2]*q[0] - r[3]*q[1];
    t[3] = r[0]*q[3] - r[1]*q[2] + r[2]*q[1] + r[3]*q[0];
    
    return t;
}

void quaternionInv(float *q) {
    q[1] = -q[1];
    q[2] = -q[2];
    q[3] = -q[3];
}

float *quaternionZero() {
    float *qZero = (float*) calloc(4, sizeof(float));
    qZero[0] = 1;
    qZero[1] = 0;
    qZero[2] = 0;
    qZero[3] = 0;

    return qZero;
}

float quaternionModule(float *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
}