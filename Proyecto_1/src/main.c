
#include "mpu.h"
#include "i2c.h"

int main() {

    uint8_t fd = i2c_init(1, MPU_ADRESS);
    if (fd < 0){
        printf("i2c_init error");
        return -1;
    }

    if (mpu_init(fd, MPU_ADRESS) != 0) {
        printf("Error inicializando el MPU6050\n");
        return -1;
    }

    printf("Leyendo datos del MPU6050...\n");

    while (1) {
        Mpu_accel accel;

        if (mpu_read_accel(fd, MPU_ADRESS, &accel) < 0) {
            printf("Error al leer la aceleración\n");
            break;
        }
        printf("Aceleración (g): X=%.2f  Y=%.2f  Z=%.2f\n", accel.ax, accel.ay, accel.az);
    }
    close(fd);
    return 0;
}