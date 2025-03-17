#ifndef CLDTDC_H
 #define CLDTDC_H
 #include <stdint.h>
 #include <stdio.h>
 #include "i2c.h"

 #define TCS34725_ADDR 0x29  // Dirección I2C del sensor
 #define ENABLE_REG 0x00
 #define ATIME_REG 0x01
 #define CONTROL_REG 0x0F
 #define STATUS_REG 0x13
 #define CDATA_REG 0x14
 #define RDATA_REG 0x16
 #define GDATA_REG 0x18
 #define BDATA_REG 0x1A
 
#endif
