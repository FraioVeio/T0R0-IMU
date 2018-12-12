
extern "C" {
#include "IMU_metric.h"
#include "quaternions.h"
}
#include "MahonyAHRS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "configuration_parser.h"


int main() {
    IMUMET_init();

    if(false) { // Magnetometer to file for fine calibration
        FILE *f = fopen("MagOut.txt", "w+");
        printf("Inizio in 3 sec\n");
        IMU_delay(3000);
        int mag[3];

        for(int i=0;i<2000;i++) {
            mag[0] = IMUMET_magX();
            mag[1] = IMUMET_magY();
            mag[2] = IMUMET_magZ();

            fprintf(f, "%i,%i,%i\n", mag[0], mag[1], mag[2]);
            printf("%i,%i,%i\n", mag[0], mag[1], mag[2]);

            IMU_delay(100);
        }

        printf("Fine\n");
        fclose(f);
        return 0;
    }

    if(true) { // true: keep calibration, false: calibrate
        int gx, gy, gz, mx, my, mz;
        conf_readInt(&gx, (char*)"IMU_calibration.conf", (char*)"gyoffX");
        conf_readInt(&gy, (char*)"IMU_calibration.conf", (char*)"gyoffY");
        conf_readInt(&gz, (char*)"IMU_calibration.conf", (char*)"gyoffZ");
        conf_readInt(&mx, (char*)"IMU_calibration.conf", (char*)"magoffX");
        conf_readInt(&my, (char*)"IMU_calibration.conf", (char*)"magoffY");
        conf_readInt(&mz, (char*)"IMU_calibration.conf", (char*)"magoffZ");

        IMURAW_gyoffX = (short) gx;
        IMURAW_gyoffY = (short) gy;
        IMURAW_gyoffZ = (short) gz;
        IMURAW_magoffX = (short) mx;
        IMURAW_magoffY = (short) my;
        IMURAW_magoffZ = (short) mz;
    } else {
        printf("Fermo che calibro gyro 5s\n");
        IMURAW_gyroCalibration(10);
        printf("OK ora muoviti su tutti gli assi\n");
        //IMURAW_magnetometerCalibration(20);

        conf_writeInt((char*)"IMU_calibration.conf", (char*)"gyoffX", IMURAW_gyoffX);
        conf_writeInt((char*)"IMU_calibration.conf", (char*)"gyoffY", IMURAW_gyoffY);
        conf_writeInt((char*)"IMU_calibration.conf", (char*)"gyoffZ", IMURAW_gyoffZ);
        //conf_writeInt((char*)"IMU_calibration.conf", (char*)"magoffX", IMURAW_magoffX);
        //conf_writeInt((char*)"IMU_calibration.conf", (char*)"magoffY", IMURAW_magoffY);
        //conf_writeInt((char*)"IMU_calibration.conf", (char*)"magoffZ", IMURAW_magoffZ);
    }

    printf("%i %i %i   ;   %i %i %i\n\n", IMURAW_magoffX, IMURAW_magoffY, IMURAW_magoffZ, IMURAW_gyoffX, IMURAW_gyoffY, IMURAW_gyoffZ);

    Mahony filter;
    filter.begin(100);

    long microold = IMU_micros();
    int count = 0;

    float *qZero = quaternionZero();

    float magxm[10], magym[10], magzm[10];

    memset(magxm, 0, sizeof(float)*10);
    memset(magym, 0, sizeof(float)*10);
    memset(magzm, 0, sizeof(float)*10);

    printf("Posizionati allo 0 in 60 secondi\n");

    float gy[3], acc[3], mag[3];
    float ang0 = 0;
    while(1) {
        float micro;

        gy[0] = IMUMET_gyDegX();
        gy[1] = IMUMET_gyDegY();
        gy[2] = IMUMET_gyDegZ();
        acc[0] = IMUMET_accX();
        acc[1] = IMUMET_accY();
        acc[2] = IMUMET_accZ();
        mag[0] = IMUMET_magX();
        mag[1] = IMUMET_magY();
        mag[2] = IMUMET_magZ();

        for(int i=9;i>=1;i--) {
            magxm[i] = magxm[i-1];
            magym[i] = magym[i-1];
            magzm[i] = magzm[i-1];
        }
        magxm[0] = mag[0];
        magym[0] = mag[1];
        magzm[0] = mag[2];
        mag[0] = mag[1] = mag[2] = 0;
        for(int i=0;i<10;i++) {
            mag[0] += magxm[i]/10;
            mag[1] += magym[i]/10;
            mag[2] += magzm[i]/10;
        }

        float angle = acos(mag[1]/sqrt(mag[0]*mag[0]+mag[1]*mag[1]));
        if(mag[0]<0)
            angle = 2*M_PI-angle;
        angle *= 360/(2*M_PI);
        
        while((micro = IMU_micros())-microold < 10000);

        filter.update(gy[0], gy[1], gy[2], acc[0], acc[1], acc[2], mag[0], mag[1], mag[2]);
        
        if(count == 5*6000) {
            printf("Zero salvato\n");
            float *raw = filter.getQuaternionRaw();
            
            filter.setZero(raw);

            free(raw);
            ang0 = angle;
        }

        float roll, pitch, yaw;

        roll = filter.getRoll();
        pitch = filter.getPitch();
        yaw = filter.getYaw();

        printf("%f\t\t%f\t\t%f\t\t%f\n%f\t\t%f\t\t%f\t\t%f\n\n", roll, pitch, yaw, sqrt(mag[0]*mag[0]+mag[1]*mag[1]+mag[2]*mag[2]), mag[0], mag[1], mag[2], angle-ang0+180);

        microold = micro;

        count ++;
    }

    return 0;
}