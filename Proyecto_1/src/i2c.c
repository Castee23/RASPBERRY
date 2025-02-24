#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include "i2c.h"

int i2c_init(int addr){
    char i2cFile[15];
    sprintf(i2cFile, "/dev/i2c-%d", 1);
    int fd = open(i2cFile, O_RDWR);
    ioctl(fd, I2C_SLAVE, addr);
    return fd;
}
int i2c_write(int fd, uint8_t reg, uint8_t *data, int len, int addr){
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    if (len > 0 && data != NULL) {
        memcpy(&buffer[1], data, len);
    }

    Messages messages;
    messages.addr = addr; // direccion del dispositivo ??
    messages.flags = 0;   // escritura
    messages.len = len+1;  
    messages.buf  = buffer; // lo q se va aescribir

    if (ioctl(fd, I2C_RDWR, &messages) < 0) {
        perror("Error en i2c_write_data");
        return -1;
    }
}

int i2c_read_write(int fd, uint8_t reg, uint8_t *data, int len, int addr) {
    Messages messages[2];
    Packets packet;
    
    // Primer mensaje: se envía el registro de interés
    messages[0].addr  = addr;
    messages[0].flags = 0;   // Modo escritura
    messages[0].len   = 1;
    messages[0].buf   = &reg;
    
    // Segundo mensaje: se leen 'len' bytes desde ese registro
    messages[1].addr  = addr;
    messages[1].flags = 1;  // Modo lectura
    messages[1].len   = len;
    messages[1].buf   = data;
    
    // Agrupamos los dos mensajes en una sola transacción
    packet.msgs = messages;
    packet.nmsgs = 2;
    
    // Realizamos la transacción I2C
    if (ioctl(fd, I2C_RDWR, &packet) < 0) {
        perror("Error en i2c_read_data");
        return -1;
    }
    return 0;
}