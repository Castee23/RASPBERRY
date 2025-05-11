 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/i2c-dev.h>
 #include <linux/i2c.h>
 #include <string.h>

// Defines MPU
 #define MPU_ADRESS      0x68
 #define DEVICE_RESET    0x6B
 #define ACCEL_XOUT_H    0x3B

// Defines TCS
#define TCS34725_ADDR  0x29
#define COMMAND_BIT    0x80
#define ENABLE_REG     0x00
#define ATIME_REG      0x01
#define CONTROL_REG    0x0F
#define CDATAL_REG     0x14


 typedef struct
 {
    float ax;
    float ay;
    float az;
 }Mpu_accel;


 int mpu_init(int fd, int addr);
 int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel);

 int tcs_init(int fd);
 void read_tcs34725(int fd);

 int i2c_init(int dev, int addr);


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

    if (tcs_init(fd) != 0) {
        printf("Error inicializando el TCS34725\n");
        return -1;
    }

    printf("Leyendo datos...\n");

    while (1) {
        Mpu_accel accel;
        read_tcs34725(fd);
        mpu_read_acceleration(fd, MPU_ADRESS, &accel);
        printf("Aceleración (g): X=%.2f  Y=%.2f  Z=%.2f\n", accel.ax, accel.ay, accel.az);
        sleep(1);
    }
    close(fd);
    return 0;
}
