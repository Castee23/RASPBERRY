#ifndef __MPU6050_H
#define __MPU6050_H

#define ADRESS          0x68
#define DEVICE_RESET    0x6B
#define ADRESS_0        0x00

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

#endif
