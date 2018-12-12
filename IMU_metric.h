#ifndef IMU_METRIC_HEADER
#define IMU_METRIC_HEADER

#include "IMU_raw.h"

void IMUMET_init();
void IMUMET_accelerometerCalibration(int seconds);
float IMUMET_accX();
float IMUMET_accY();
float IMUMET_accZ();
float IMUMET_gyDegX();
float IMUMET_gyDegY();
float IMUMET_gyDegZ();
float IMUMET_gyRadX();
float IMUMET_gyRadY();
float IMUMET_gyRadZ();
float IMUMET_magX();
float IMUMET_magY();
float IMUMET_magZ();


float IMUMET_accDivider;
float IMUMET_gyDivider;
float IMUMET_gyMultiplierX, IMUMET_gyMultiplierY, IMUMET_gyMultiplierZ;

#define IMUMET_g 9.80665

#endif