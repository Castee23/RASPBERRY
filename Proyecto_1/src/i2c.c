
#include "i2c.h"

int i2c_init(int dev, int addr){
    char i2cFile[16];
    sprintf(i2cFile, "/dev/i2c-%d", dev);
    uint8_t fd = open(i2cFile, O_RDWR);
    if (fd < 0) {
        perror("Error al abrir el bus I2C");
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        perror("Error al configurar la dirección I2C_SLAVE");
        close(fd);
        return -1;
    }
    return fd;
}
int i2c_write(int fd, uint8_t reg, uint8_t *data, int len, int addr){
    uint8_t buffer[len + 1];
    buffer[0] = reg;                    // reg = registro al q se quiere acceder
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
    return 0;
}

int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr) {
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