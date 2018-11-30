#include "IMU_metric.h"
#include <stdlib.h>
#include <math.h>

void IMUMET_init() {
    IMURAW_connect();
    IMURAW_initialize();
    IMURAW_accRange(IMU_4g);
    IMURAW_gyRange(IMU_500dps);

    IMUMET_accDivider = 801.543262;
    IMUMET_gyDivider = 65.536;
}

void IMUMET_accelerometerCalibration(int seconds) {
    long microin = micros();


    double totals = 0;
    int count = 0;

    while(((micros()-microin)/1000000) < seconds) {
        short accx = IMURAW_accX(), accy = IMURAW_accY(), accz = IMURAW_accZ();

        totals += sqrt((double)accx*accx + (double)accy*accy + (double)accz*accz);

        count ++;
        
        delay(10);
    }

    IMUMET_accDivider = (totals/count)/IMUMET_g;
}

double IMUMET_accX() {
    return (double) IMURAW_accX()/IMUMET_accDivider;
}

double IMUMET_accY() {
    return (double) IMURAW_accY()/IMUMET_accDivider;
}

double IMUMET_accZ() {
    return (double) IMURAW_accZ()/IMUMET_accDivider;
}

double IMUMET_gyDegX() {
    return (double) IMURAW_gyX()/IMUMET_gyDivider;
}

double IMUMET_gyDegY() {
    return (double) IMURAW_gyY()/IMUMET_gyDivider;
}

double IMUMET_gyDegZ() {
    return (double) IMURAW_gyZ()/IMUMET_gyDivider;
}

double IMUMET_gyRadX() {
    return IMUMET_gyDegX()*180/M_PI;
}

double IMUMET_gyRadY() {
    return IMUMET_gyDegY()*180/M_PI;
}

double IMUMET_gyRadZ() {
    return IMUMET_gyDegZ()*180/M_PI;
}

double IMUMET_magX() {
    return IMURAW_magX()*IMURAW_magScaleX;
}

double IMUMET_magY() {
    return IMURAW_magY()*IMURAW_magScaleY;
}

double IMUMET_magZ() {
    return IMURAW_magZ()*IMURAW_magScaleZ;
}