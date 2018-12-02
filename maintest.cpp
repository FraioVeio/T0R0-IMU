
extern "C" {
#include "IMU_metric.h"
}
#include "MahonyAHRS.h"
#include <stdio.h>
#include <math.h>

int main() {
    IMUMET_init();
    printf("Fermo che calibro gyro\n");
    IMURAW_gyroCalibration(5);
    printf("OK ora muoviti su tutti gli assi\n");
    IMURAW_magnetometerCalibration(10);

    Mahony filter;
    filter.begin(100);

    long microold = IMU_micros();

    while(1) {
        float gy[3], acc[3], mag[3];

        gy[0] = IMUMET_gyDegX();
        gy[1] = IMUMET_gyDegY();
        gy[2] = IMUMET_gyDegZ();
        acc[0] = IMUMET_accX();
        acc[1] = IMUMET_accY();
        acc[2] = IMUMET_accZ();
        mag[0] = IMUMET_magX();
        mag[1] = IMUMET_magY();
        mag[2] = IMUMET_magZ();
        float micro;

        filter.update(gy[0], gy[1], gy[2], acc[0], acc[1], acc[2], mag[0], mag[1], mag[2]);

        float roll, pitch, yaw;

        roll = filter.getRoll();
        pitch = filter.getPitch();
        yaw = filter.getYaw();

        printf("%f\t\t%f\t\t%f\n", roll, pitch, yaw);

        microold = micro;
    }

    return 0;
}