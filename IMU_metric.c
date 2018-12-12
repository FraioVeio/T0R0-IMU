#include "IMU_metric.h"
#include <stdlib.h>
#include <math.h>

void IMUMET_init() {
    IMURAW_connect();
    IMURAW_initialize();
    IMURAW_accRange(IMU_4g);
    IMURAW_gyRange(IMU_1000dps);

    IMUMET_accDivider = 801.543262;
    IMUMET_gyDivider = 32.768;

    IMUMET_gyMultiplierX = 1;
    IMUMET_gyMultiplierY = 0.85;
    IMUMET_gyMultiplierZ = 1;
}

void IMUMET_accelerometerCalibration(int seconds) {
    long microin = micros();


    float totals = 0;
    int count = 0;

    while(((micros()-microin)/1000000) < seconds) {
        short accx = IMURAW_accX(), accy = IMURAW_accY(), accz = IMURAW_accZ();

        totals += sqrt((float)accx*accx + (float)accy*accy + (float)accz*accz);

        count ++;
        
        delay(10);
    }

    IMUMET_accDivider = (totals/count)/IMUMET_g;
}

float IMUMET_accX() {
    return (float) IMURAW_accX()/IMUMET_accDivider;
}

float IMUMET_accY() {
    return (float) IMURAW_accY()/IMUMET_accDivider;
}

float IMUMET_accZ() {
    return (float) IMURAW_accZ()/IMUMET_accDivider;
}

float IMUMET_gyDegX() {
    return (float) IMURAW_gyX()/IMUMET_gyDivider*IMUMET_gyMultiplierX;
}

float IMUMET_gyDegY() {
    return (float) IMURAW_gyY()/IMUMET_gyDivider*IMUMET_gyMultiplierY;
}

float IMUMET_gyDegZ() {
    return (float) IMURAW_gyZ()/IMUMET_gyDivider*IMUMET_gyMultiplierZ;
}

float IMUMET_gyRadX() {
    return IMUMET_gyDegX()/180*M_PI;
}

float IMUMET_gyRadY() {
    return IMUMET_gyDegY()/180*M_PI;
}

float IMUMET_gyRadZ() {
    return IMUMET_gyDegZ()/180*M_PI;
}

float IMUMET_magX() {
    return IMURAW_magX()*IMURAW_magScaleX;
}

float IMUMET_magY() {
    return IMURAW_magY()*IMURAW_magScaleY;
}

float IMUMET_magZ() {
    return IMURAW_magZ()*IMURAW_magScaleZ;
}