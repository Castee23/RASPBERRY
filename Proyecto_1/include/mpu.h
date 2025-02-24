#ifndef __MPU6050_H
 #define __MPU6050_H
 #include <stdint.h>
 #include <stdio.h>
 #include "i2c.h"

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
 #define GYRO_CONFIG     0x43
 #define GYRO_XOUT_H     0x44 
 #define GYRO_XOUT_L     0x45
 #define GYRO_YOUT_H     0x46
 #define GYRO_YOUT_L     0x47
 #define GYRO_ZOUT_H     0x48

 typedef struct 
 {
    uint16_t ax;
    uint16_t ay;
    uint16_t az;
 }Mpu_accel;

 int mpu_init(int fd, int addr);
 int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel);
 

#endif
