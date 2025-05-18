 #include <stdint.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/i2c-dev.h>
 #include <linux/i2c.h>
 #include <string.h>

 int i2c_write(int fd, uint8_t reg, uint8_t *data, int len, int addr);
 int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr);

 #define MPU_ADRESS      0x68
 #define DEVICE_RESET    0x6B
 #define ACCEL_XOUT_H    0x3B


 typedef struct
 {
    float ax;
    float ay;
    float az;
 }Mpu_accel;

 int mpu_init(int fd, int addr);
 int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel);


int mpu_init(int fd, int addr){
  uint8_t data = 0x00;    //valor para despertar al mpu
  if (i2c_write(fd, DEVICE_RESET, &data, 1, addr)){
    printf("mpu_init error");
    return -1;
  }
  return 0;
}

int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel){
  uint8_t data[6]; // cada aceleracion ocupa 2 bytes

  if (i2c_read(fd, ACCEL_XOUT_H, data, 6, addr)){
    printf("mpu_init error");
    return -1;
  }

  //Unir los bytes altos con los bajos
  int16_t raw_x = (data[0] << 8) | data[1];
  int16_t raw_y = (data[2] << 8) | data[3];
  int16_t raw_z = (data[4] << 8) | data[5];

  //Convertir de raw a float dependiendo de g: 2g
  accel->ax = raw_x/16384.0;
  accel->ay = raw_y/16384.0;
  accel->az = raw_z/16384.0;

  return 0;

}
