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
 int i2c_write(int fd, uint8_t reg, uint8_t *data, int len, int addr);
 int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr);
#endif