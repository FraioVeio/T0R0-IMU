#ifndef IMU_METRIC_HEADER
#define IMU_METRIC_HEADER

#include "IMU_raw.h"

void IMUMET_init();
void IMUMET_accelerometerCalibration(int seconds);
double MUMET_accX();
double IMUMET_accY();
double IMUMET_accZ();
double IMUMET_gyDegX();
double IMUMET_gyDegY();
double IMUMET_gyDegZ();
double IMUMET_gyRadX();
double IMUMET_gyRadY();
double IMUMET_gyRadZ();
double IMUMET_magX();
double IMUMET_magY();
double IMUMET_magZ();


double IMUMET_accDivider;
double IMUMET_gyDivider;


#define IMUMET_g 9.80665

#endif