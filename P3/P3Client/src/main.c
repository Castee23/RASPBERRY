 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/ioctl.h>
 #include <linux/i2c-dev.h>
 #include <linux/i2c.h>
 #include <string.h>
 #include <sys/types.h>
 #include <arpa/inet.h>
 #include <sys/socket.h>
 #include<netinet/in.h>

 #define PORT 5000
 #define MAXLINE 1000

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
 void read_tcs34725(int fd, int sockfd, struct sockaddr_in servaddr);

 int i2c_init(int dev, int addr);


int main() {
	char buffer[100];
	//char *message;
	int sockfd, n;
	struct sockaddr_in servaddr;

	// clear servaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr("192.168.227.33");
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;

	// create datagram socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// connect to server
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		exit(0);
	}

	//Initiate i2c
    uint8_t fd = i2c_init(1, MPU_ADRESS);
    if (fd < 0){
        printf("i2c_init error");
        return -1;
    }

    //Initiate MPU
    if (mpu_init(fd, MPU_ADRESS) != 0) {
        printf("Error inicializando el MPU6050\n");
        return -1;
    }

    //Initiate TCS
    if (tcs_init(fd) != 0) {
        printf("Error inicializando el TCS34725\n");
        return -1;
    }

    printf("Leyendo datos...\n");

    while (1) {
        Mpu_accel accel;
        read_tcs34725(fd, sockfd, servaddr);
        mpu_read_acceleration(fd, MPU_ADRESS, &accel);

        sprintf(buffer, "\nAceleración (g): X=%.2f  Y=%.2f  Z=%.2f", accel.ax, accel.ay, accel.az);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)NULL, sizeof(servaddr));

        sleep(1);
    	//recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    	//puts(buffer);
    }
	close(sockfd);
    close(fd);
    return 0;
}
