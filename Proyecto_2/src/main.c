
#include "cltdc.h"
#include "i2c.h"

int main() {
    char *i2c_dev = "/dev/i2c-1";
    i2c_fd = open(i2c_dev, O_RDWR);
    if (i2c_fd < 0) {
        perror("Error abriendo I2C");
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, TCS34725_ADDR) < 0) {
        perror("Error configurando dirección I2C");
        return -1;
    }

    tcs34725_init();
    read_rgb_values();

    close(i2c_fd);
    return 0;
}