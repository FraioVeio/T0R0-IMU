#ifndef IMU_RAW_HEADER
#define IMU_RAW_HEADER
#include "IMU_i2c.h"

#define IMU_ADDRESS 0x68

void IMURAW_connect();
void IMURAW_initialize();
void IMURAW_accRange(unsigned char mode);
void IMURAW_gyRange(unsigned char mode);
short IMURAW_accX();
short IMURAW_accY();
short IMURAW_accZ();
short IMURAW_gyX();
short IMURAW_gyY();
short IMURAW_gyZ();
short IMURAW_magX();
short IMURAW_magY();
short IMURAW_magZ();

char* IMURAW_inertial();

void IMURAW_magnetometerCalibration(int seconds);
void IMURAW_gyroCalibration(int seconds);
void IMURAW_gyOffset(short x, short y, short z);
void IMURAW_accOffset(short x, short y, short z);

void IMURAW_writeAK8963Register(int address, char data);
char IMURAW_readAK8963Register(int address);
void IMURAW_readAK8963Registers(int address, char len);

int IMURAW_connectionIMU;
short IMURAW_accX_data, IMURAW_accY_data, IMURAW_accZ_data, IMURAW_gyX_data, IMURAW_gyY_data, IMURAW_gyZ_data;

// Gestisci meglio i valori di default
double IMURAW_magScaleX, IMURAW_magScaleY, IMURAW_magScaleZ;
short IMURAW_magoffX, IMURAW_magoffY, IMURAW_magoffZ;
short IMURAW_gyoffX, IMURAW_gyoffY, IMURAW_gyoffZ;


// Settings
// Accelerometer range
#define IMU_2g  0B00000000
#define IMU_4g  0B00001000
#define IMU_8g  0B00010000
#define IMU_16g 0B00011000

// Gyro range (dps = degrees per second)
#define IMU_250dps  0B00000000
#define IMU_500dps  0B00001000
#define IMU_1000dps 0B00010000
#define IMU_2000dps 0B00011000


#endif