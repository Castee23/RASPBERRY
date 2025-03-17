
#include "i2c.h"

void i2c_write(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    write(i2c_fd, buffer, 2);
}

uint16_t i2c_read_word(uint8_t reg) {
    uint8_t buffer[2];
    write(i2c_fd, &reg, 1);
    read(i2c_fd, buffer, 2);
    return (buffer[1] << 8) | buffer[0];
}