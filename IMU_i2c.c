#include "IMU_i2c.h"

int IMU_i2cSetup(int id) {
    return wiringPiI2CSetup (id);
}

int IMU_i2cReadReg8(int fd, int reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

int IMU_i2cWriteReg8(int fd, int reg, int data) {
    return wiringPiI2CWriteReg8(fd, reg, data);
}

long IMU_micros() {
    return micros();
}

void IMU_delay(int millis) {
    delay(millis);
}