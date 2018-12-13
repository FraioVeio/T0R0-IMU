#include "IMU_raw.h"
#include <stdlib.h>
#include <math.h>


void IMURAW_connect() {
    IMURAW_connectionIMU = wiringPiI2CSetup(IMU_ADDRESS);
}

void IMURAW_initialize() {
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x6B, 0B00000000); // Wake up IMU
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x0A, 0B00010000); // Wake up COMPASS

    // Enable i2c master mode
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x6A, 0x20);
    // Set the I2C bus speed to 400 kHz
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x24, 0x0D);

    // Power down AK8963
    IMURAW_writeAK8963Register(0x0A, 0x00);

    // Reset AK8963
    IMURAW_writeAK8963Register(0x0B, 0x01);

    // Set AK8963 to Fuse ROM access
    IMURAW_writeAK8963Register(0x0A, 0x0F);
    delay(100); // Wait for mode change

    // Get sensitivity adj for magnetometer
    char asax = IMURAW_readAK8963Register(0x10);
    char asay = IMURAW_readAK8963Register(0x11);
    char asaz = IMURAW_readAK8963Register(0x12);
    IMURAW_magScaleX = ((asax-128)*0.5)/128.0+1;    // Mag scale not used because inaccurate
    IMURAW_magScaleY = ((asax-128)*0.5)/128.0+1;
    IMURAW_magScaleZ = ((asax-128)*0.5)/128.0+1;

    // Power down AK8963
    IMURAW_writeAK8963Register(0x0A, 0x00);
    delay(100); // Wait mode change

    // Set AK8963 to 16 bit resolution, 100 Hz update rate  0B00010110
    IMURAW_writeAK8963Register(0x0A, 0B00010110);
    delay(100); // Wait mode change

    // Instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
    IMURAW_readAK8963Registers(0x03, 7);


    IMURAW_magoffX=-217;
    IMURAW_magoffY=105;
    IMURAW_magoffZ=60;
    IMURAW_gyoffX=0;
    IMURAW_gyoffY=0;
    IMURAW_gyoffZ=0;

    IMURAW_magScaleX = 1.0400;
    IMURAW_magScaleY = 1.1484;
    IMURAW_magScaleZ = 1.2279;
    
}

void IMURAW_accRange(unsigned char mode) {
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x1C, mode);
}

void IMURAW_gyRange(unsigned char mode) {
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x1B, mode);
}

short IMURAW_accX() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x3B);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x3B)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x3C));
}

short IMURAW_accY() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x3D);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x3D)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x3E));
}

short IMURAW_accZ() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x3F);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x3F)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x40));
}

short IMURAW_gyX() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x43);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x43)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x44)) + IMURAW_gyoffX;
}

short IMURAW_gyY() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x45);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x45)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x46)) + IMURAW_gyoffY;
}

short IMURAW_gyZ() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x47);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x47)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x48)) + IMURAW_gyoffZ;
}

short IMURAW_magY() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x43);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x4A)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x49)) - IMURAW_magoffY;
}

short IMURAW_magX() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x43);
    return (short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x4C)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x4B)) - IMURAW_magoffX;
}

short IMURAW_magZ() {
    //IMU_i2cWrite(IMURAW_connectionIMU, 0x43);
    return -1*(short) (IMU_i2cReadReg8(IMURAW_connectionIMU, 0x4E)<<8|IMU_i2cReadReg8(IMURAW_connectionIMU, 0x4D)) - IMURAW_magoffZ;
}


/* WARNING: use stdlib free(ptr) after using data */
// Da finire non usare perchè funge male e fa bordello!!

/*char* IMURAW_inertial() {
    char *data = malloc(12);

    IMU_i2cWrite(IMURAW_connectionIMU, 0x3B);
    IMURAW_accX_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    IMURAW_accY_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    IMURAW_accZ_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    
    
    IMU_i2cWrite(IMURAW_connectionIMU, 0x43);
    IMURAW_gyX_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    IMURAW_gyY_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    IMURAW_gyZ_data = (short) (IMU_i2cRead(IMURAW_connectionIMU)<<8|IMU_i2cRead(IMURAW_connectionIMU));
    
    return data;
}*/


// Attenzione, non sto usando la funzione offset dell'imu xk non funge, guarda perchè!
void IMURAW_gyroCalibration(int seconds) {
    IMURAW_gyoffX = IMURAW_gyoffY = IMURAW_gyoffZ = 0;
    long microin = micros();


    long gyxs = 0, gyys = 0, gyzs = 0;
    int count = 0;

    while(((micros()-microin)/1000000) < seconds) {
        int gx = IMURAW_gyX(), gy = IMURAW_gyY(), gz = IMURAW_gyZ();

        gyxs += gx;
        gyys += gy;
        gyzs += gz;

        count ++;
        
        delay(10);
    }

    IMURAW_gyoffX = -(short) round((double) gyxs/ (double) count);
    IMURAW_gyoffY = -(short) round((double) gyys/ (double) count);
    IMURAW_gyoffZ = -(short) round((double) gyzs/ (double) count);
}

void IMURAW_magnetometerCalibration(int seconds) {
    IMURAW_magoffX = IMURAW_magoffY = IMURAW_magoffZ = 0;
    long microin = micros();

    int maxX = -65536, maxY = -65536, maxZ = -65536;
    int minX = 65536, minY = 65536, minZ = 65536;

    while(((micros()-microin)/1000000) < seconds) {
        int mx = IMURAW_magX(), my = IMURAW_magY(), mz = IMURAW_magZ();

        if(mx>maxX)
            maxX = mx;
        if(mx<minX)
            minX = mx;
        if(my>maxY)
            maxY = my;
        if(my<minY)
            minY = my;
        if(mz>maxZ)
            maxZ = mz;
        if(mz<minZ)
            minZ = mz;
        
        delay(20);
    }

    IMURAW_magoffX = (maxX+minX)/2;
    IMURAW_magoffY = (maxY+minY)/2;
    IMURAW_magoffZ = (maxZ+minZ)/2;
}


// Guarda perchè ma sembra non fungere bene
void IMURAW_gyOffset(short x, short y, short z) {
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x13, x >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x14, x & 0xFF);
    
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x15, y >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x16, y & 0xFF);
    
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x17, z >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x18, z & 0xFF);
}

void IMURAW_accOffset(short x, short y, short z) {
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x77, x >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x78, x & 0xFF);
    
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x7A, y >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x7B, y & 0xFF);
    
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x7D, z >> 8);
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x7E, z & 0xFF);
}

void IMURAW_writeAK8963Register(int address, char data) {
    // Slave 0 address of AK8963 write flag
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x25, 0x0C | 0B00000000);

    // Slave0 register address to address
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x26, address);

    // Store data to write
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x63, data);

    // Enable I2C and send 1 byte
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x27, 0B10000001);

    // Magari fai una lettura per confermare quando avrai fatto il leggi
}

char IMURAW_readAK8963Register(int address) {
    // Slave 0 address of AK8963 read flag
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x25, 0x0C | 0B10000000);

    // Slave0 register address to address
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x26, address);

    // Enable I2C and request 1 byte (last digit in hex)
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x27, 0x81);

    delay(1);   // Wait for register to fill

    // Read data
    return IMU_i2cReadReg8(IMURAW_connectionIMU, 0x49);
}

void IMURAW_readAK8963Registers(int address, char len) {
    // Slave 0 address of AK8963 read flag
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x25, 0x0C | 0B10000000);

    // Slave0 register address to address
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x26, address);

    // Enable I2C and request 7 byte (last digit in hex)
    IMU_i2cWriteReg8(IMURAW_connectionIMU, 0x27, 0B10000000 | len);

    delay(1);   // Wait for registers to fill
}