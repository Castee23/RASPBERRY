#include "cltdc.h"

void tcs34725_init() {
    i2c_write(ENABLE_REG, 0x03);  // Activa el sensor y el ADC
    i2c_write(ATIME_REG, 0xD5);   // Tiempo de integración (101 ms)
    i2c_write(CONTROL_REG, 0x01); // Ganancia 1x
}

void read_rgb_values() {
    while (1) {
        uint8_t status = i2c_read_word(STATUS_REG);
        if (status & 0x01) {  // Si hay datos listos
            uint16_t clear = i2c_read_word(CDATA_REG);
            uint16_t red = i2c_read_word(RDATA_REG);
            uint16_t green = i2c_read_word(GDATA_REG);
            uint16_t blue = i2c_read_word(BDATA_REG);

            // Evitar división por 0
            if (clear == 0) clear = 1;

            // Convertir a porcentaje (%)
            float r_percent = (red / (float)clear) * 100.0;
            float g_percent = (green / (float)clear) * 100.0;
            float b_percent = (blue / (float)clear) * 100.0;

            printf("Color en porcentaje: R=%.2f%% G=%.2f%% B=%.2f%%\n", r_percent, g_percent, b_percent);
        }
        usleep(1000000); // Espera 500ms
    }
}
