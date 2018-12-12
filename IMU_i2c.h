#ifndef IMU_RAW_I2C
#define IMU_RAW_I2C

#include "wiringPi/wiringPiI2C.h"

int IMU_i2cSetup(int id);

int IMU_i2cRead(int fd);

int IMU_i2cWrite(int fd, int data);

int IMU_i2cReadReg8(int fd, int reg);

int IMU_i2cWriteReg8(int fd, int reg, int data);

long IMU_micros();

void IMU_delay(int millis);

#endif