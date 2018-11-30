#include "IMU_metric.h"
#include <stdio.h>
#include <math.h>

int main() {
    IMUMET_init();

    long microold = micros();

    while(1) {
        long micro = micros();
        double gy[3], acc[3], mag[3];

        gy[0] = IMUMET_gyRadX();
        gy[1] = IMUMET_gyRadY();
        gy[2] = IMUMET_gyRadZ();
        acc[0] = IMUMET_accX();
        acc[1] = IMUMET_accY();
        acc[2] = IMUMET_accZ();
        mag[0] = IMUMET_magX();
        mag[1] = IMUMET_magY();
        mag[2] = IMUMET_magZ();
        
        

        printf("%lf\t\t%lf\t\t%lf\t\t%li\n", mag[0], mag[1], mag[2], micro-microold);



        microold = micro;
    }

    return 0;
}