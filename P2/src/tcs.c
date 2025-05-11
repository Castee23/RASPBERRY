#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>

 int i2c_write(int fd, uint8_t reg, uint8_t *data, int len, int addr);
 int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr);

#define TCS34725_ADDR  0x29
#define COMMAND_BIT    0x80
#define ENABLE_REG     0x00
#define ATIME_REG      0x01
#define CONTROL_REG    0x0F
#define CDATAL_REG     0x14

void read_tcs34725(int fd) {
    uint8_t data[8];

    if (i2c_read(fd, CDATAL_REG, data, 8, TCS34725_ADDR) < 0) {
        perror("Error al leer el sensor de color");
        return;
    }

    uint16_t clear = (data[1] << 8) | data[0];
    uint16_t red   = (data[3] << 8) | data[2];
    uint16_t green = (data[5] << 8) | data[4];
    uint16_t blue  = (data[7] << 8) | data[6];

    printf("Color Clear: %d, Red: %d, Green: %d, Blue: %d\n", clear, red, green, blue);
}

int tcs_init(int fd) {
	uint8_t data = 0x03;
    if (i2c_write(fd, ENABLE_REG, &data, 1, TCS34725_ADDR)){
      printf("tcs_init error");
      return -1;
    }
    data = 0xD5;
    if (i2c_write(fd, ATIME_REG, &data, 1, TCS34725_ADDR)){
      printf("tcs_init error");
      return -1;
    }
    data = 0x01;
    if (i2c_write(fd, CONTROL_REG, &data, 1, TCS34725_ADDR)){
      printf("tcs_init error");
      return -1;
    }
    return 0;
}
