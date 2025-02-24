#include <stdio.h>
#include <unistd.h>
#include "mpu.h"

int main() {
    if (mpu6050_init() != 0) {
        printf("Error inicializando el MPU6050\n");
        return -1;
    }

    printf("Leyendo datos del MPU6050...\n");

    while (1) {
        float ax, ay, az;
        mpu6050_read_acceleration(&ax, &ay, &az);
        printf("Aceleración: X=%.2f, Y=%.2f, Z=%.2f\n", ax, ay, az);
        usleep(500000);
    }

    return 0;
}