#include "Mpu6050.h"
#include <stdio.h>

int mpu(void){
  int fd;
  uint8_t data[6];
  
  i2c_read(fd, ACCEL_XOUT_H, data, 6, ADRESS);
  i2c_read(fd, ACCEL_XOUT_L, data, 6, ADRESS);
  
  i2c_read(fd, ACCEL_YOUT_H, data, 6, ADRESS);
  i2c_read(fd, ACCEL_YOUT_L, data, 6, ADRESS);

  i2c_read(fd, ACCEL_ZOUT_H, data, 6, ADRESS);
  i2c_read(fd, ACCEL_ZOUT_L, data, 6, ADRESS);

  int16_t accel_x = (data[0] << 8) | data[1];
  int16_t accel_y = (data[2] << 8) | data[3];
  int16_t accel_z = (data[4] << 8) | data[5];

  int x = accel_x / 16384.0;
  int y = accel_y / 16384.0;
  int z = accel_z / 16384.0;
  
  
}
