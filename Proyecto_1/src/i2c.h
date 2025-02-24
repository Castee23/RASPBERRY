#ifndef I2C_H
 #define I2C_H
 
 
 int i2c_init(int addr);
 typedef struct {
    int addr;
    int flags;
    int len;
    int reg_addr;
    int buf;
 } Messages;
 typedef struct {
    Messages* msgs;
    int nmsgs;
 } 
 Packets;
#endif