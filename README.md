Aquí tienes tu README traducido al inglés y formateado en Markdown (.md):

# Project 1: MPU-6050

## Connecting Raspberry Pi with the VM
- The VM and the Raspberry Pi (RPI) are on the same local network, meaning they can communicate using their IPv4 addresses.
- The SSH protocol allows establishing a secure connection between both devices, transmitting data in an encrypted manner.
- By using the following command, the VM attempts to connect to the RPI using its local IP address and system user. If everything is correctly configured on the RPI, the connection is established.

  ```sh
  ssh user@IPv4-of-RPI

	•	Once connected, the VM terminal acts as if you were directly using the RPI terminal. You can execute commands, access files, etc.

I2C Protocol

I2C (Inter-Integrated Circuit) is a synchronous serial communication bus for communication between multiple integrated devices within the same system.

Key Features:
	•	Synchronous serial communication: Uses a clock signal (SCL) to synchronize data transmission on the data line (SDA).
	•	Multi-master and multi-slave protocol: Multiple master devices and multiple slaves can exist on the same bus.
	•	Unique addresses: Each slave has a 7-bit or 10-bit address to be identified on the bus.

I2C Communication Protocol:
	1.	Start Condition:
	•	The master initiates communication by pulling SDA low while SCL remains high. This indicates to all slaves that a transmission is about to begin.
	2.	Slave Addressing:
	•	The master sends a 7-bit address followed by a read/write bit:
	•	0: Write to the slave.
	•	1: Read from the slave.
	3.	Acknowledge (ACK/NACK):
	•	The addressed slave responds with an ACK bit (0) if it recognizes the address. If not, it returns a NACK (1).
	4.	Data Transfer:
	•	After each byte transmission, an ACK is sent to confirm successful reception.
	5.	Stop Condition:
	•	The master ends communication by changing SDA from low to high while SCL is high, releasing the bus for other communications.

Project Code

Folder Structure:
	1.	docs/
	•	General documentation, diagrams, and manuals.
	2.	include/
	•	Header files: function prototypes and constant definitions.
	3.	src/
	•	Source code in C:
	•	i2c.c: Low-level I2C communication.
	•	mpu.c: Functions for handling the MPU-6050 sensor.
	•	main.c: Entry point and main program logic.
	4.	Makefile
	•	Defines how the project is compiled and linked.
	5.	README.md
	•	Usage guide, compilation instructions, and general project explanation.

Libraries Used
	•	stdlib.h
	•	Memory management, data conversion, and program termination functions (exit()).
	•	stdint.h
	•	Fixed-size integer types (e.g., uint8_t) essential for I2C data handling.
	•	string.h
	•	String and memory manipulation functions (e.g., memcpy() for copying data to buffers).
	•	fcntl.h
	•	File control functions (open() for accessing the I2C device).
	•	unistd.h
	•	Unix standard functions (close(), read(), write()).
	•	sys/ioctl.h
	•	Allows device-specific control operations via ioctl().
	•	linux/i2c-dev.h
	•	Defines I2C-specific structures and constants (struct i2c_msg, I2C_SLAVE).

Functions in i2c.c

i2c_init
	1.	Defines a buffer i2cFile[16] to store the I2C device path.
	2.	Constructs the device path by appending i2c-1 (which contains acceleration data) using sprintf().
	3.	Opens the I2C device file using open() in read/write mode and verifies for errors.
	4.	Configures the slave address using ioctl():
	•	ioctl(fd, I2C_SLAVE, addr);
	•	fd: Connection to /dev/i2c-1.
	•	I2C_SLAVE: Linux constant for I2C slave configuration.
	•	Once executed, the slave address remains associated with the file descriptor until closed.

int i2c_init(int dev, int addr) {
    char i2cFile[16];
    sprintf(i2cFile, "/dev/i2c-%d", dev);
    uint8_t fd = open(i2cFile, O_RDWR);
    if (fd < 0) {
        perror("Error opening I2C bus");
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        perror("Error configuring I2C_SLAVE address");
        close(fd);
        return -1;
    }
    return fd;
}

i2c_write
	•	Uses a buffer to store the data to be transmitted.
	•	Configures two structures to create the message.
	•	Calls ioctl(fd, I2C_RDWR, &packets); to execute the transaction.

int i2c_write(int fd, uint8_t reg, const uint8_t *data, int len, int addr) {
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    if (len > 0 && data != NULL) {
        memcpy(&buffer[1], data, len);
    }

    struct i2c_msg message;
    struct i2c_rdwr_ioctl_data packets;

    message.addr  = addr;
    message.flags = 0;
    message.len   = len + 1;
    message.buf   = buffer;

    packets.msgs  = &message;
    packets.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Error in i2c_write_data (ioctl I2C_RDWR)");
        return -1;
    }

    return 0;
}

i2c_read
	1.	Sends a write message containing the register to read.
	2.	Follows with a read message for the requested number of bytes.
	3.	Executes the transaction using ioctl().

int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr) {
    struct i2c_msg messages[2];
    struct i2c_rdwr_ioctl_data packet;
    
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 1;
    messages[0].buf   = &reg;
    
    messages[1].addr  = addr;
    messages[1].flags = 1;
    messages[1].len   = len;
    messages[1].buf   = data;
    
    packet.msgs  = messages;
    packet.nmsgs = 2;
    
    if (ioctl(fd, I2C_RDWR, &packet) < 0) {
        perror("Error in i2c_read_data");
        return -1;
    }
    return 0;
}

Functions in mpu.c

mpu_init

Wakes up the MPU-6050 by writing 0x00 to register 0x6B.

int mpu_init(int fd, int addr) {
    uint8_t data = 0x00;
    if (i2c_write(fd, DEVICE_RESET, &data, 1, addr)) {
        printf("mpu_init error");
        return -1;
    }
    return 0;
}

mpu_read_acceleration

Reads 6 bytes of acceleration data, processes them into float values, and normalizes them by dividing by 16384.0.

int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel) {
    uint8_t data[6];

    if (i2c_read(fd, ACCEL_XOUT_H, data, 6, addr)) {
        printf("mpu_read_acceleration error");
        return -1;
    }

    int16_t raw_x = (data[0] << 8) | data[1];
    int16_t raw_y = (data[2] << 8) | data[3];
    int16_t raw_z = (data[4] << 8) | data[5];

    accel->ax = raw_x / 16384.0;
    accel->ay = raw_y / 16384.0;
    accel->az = raw_z / 16384.0;

    return 0;
}

This should be ready to use as a README.md file for your GitHub project!
