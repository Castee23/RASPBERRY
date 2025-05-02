#ifndef MPU_H
 #define MPU_H
 #include <stdint.h>
 #include <stdio.h>

 #define MPU_ADRESS      0x68
 #define DEVICE_RESET    0x6B

 #define ACCEL_CONFIG    0x1C
 #define ACCEL_XOUT_H    0x3B
 #define ACCEL_XOUT_L    0x3C
 #define ACCEL_YOUT_H    0x3D
 #define ACCEL_YOUT_L    0x3E
 #define ACCEL_ZOUT_H    0x3F
 #define ACCEL_ZOUT_L    0x40

 #define TEMP_OUT        0x41

 #define GYRO_CONFIG     0x1B
 #define GYRO_XOUT_H     0x44
 #define GYRO_XOUT_L     0x45
 #define GYRO_YOUT_H     0x46
 #define GYRO_YOUT_L     0x47
 #define GYRO_ZOUT_H     0x48

 typedef struct
 {
    float ax;
    float ay;
    float az;
 }Mpu_accel;

 int mpu_init(int fd, int addr);
 int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel);


#endif

#ifndef I2C_H
 #define I2C_H
 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/i2c-dev.h>
 #include <linux/i2c.h>
 #include <string.h>

 int i2c_init(int dev, int addr);
#endif

int main() {

    uint8_t fd = i2c_init(1, MPU_ADRESS);
    if (fd < 0){
        printf("i2c_init error");
        return -1;
    }

    if (mpu_init(fd, MPU_ADRESS) != 0) {
        printf("Error inicializando el MPU6050\n");
        return -1;
    }

    printf("Leyendo datos del MPU6050...\n");

    while (1) {
        Mpu_accel accel;

        if (mpu_read_acceleration(fd, MPU_ADRESS, &accel) < 0) {
            printf("Error al leer la aceleración\n");
            break;
        }
        printf("Aceleración (g): X=%.2f  Y=%.2f  Z=%.2f\n", accel.ax, accel.ay, accel.az);
        sleep(3);
    }
    close(fd);
    return 0;
}
