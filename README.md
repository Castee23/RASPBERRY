# Project 1: MPU-6050

## Raspberry PI Connection with the VM

- The VM and the RPI are on the same local network, which means they can communicate via their IPv4 addresses.
- The SSH protocol allows establishing a secure connection between both devices, transmitting data in encrypted form.
- By using this command, the VM attempts to connect to the RPI using its local IP address and the system user; if everything is properly configured on the RPI, the connection is established.
  - The command is executed from the VM with the RPI's user:
    ```bash
    ssh user@IPv4-of-RPI
    ```
- Once the connection is terminated, the VM's terminal acts as if you were directly using the RPI terminal. You can execute commands, access files, etc.

## I2C

I2C (Inter-Integrated Circuit) is a synchronous serial communication bus used for communication between multiple devices integrated within the same system.

### Key Characteristics

- **Synchronous serial communication:** Uses a clock (SCL) to synchronize data transmission on the data line (SDA).
- **Multi-master and multi-slave protocol:** There can be several master devices and multiple slaves on the same bus.
- **Unique addresses:** Each slave has a 7-bit or 10-bit address to be identified on the bus.

### I2C Communication Protocol

- **Start condition:**
  - The master initiates the communication by pulling SDA low while SCL remains high, indicating to all slaves on the bus that a transmission is about to begin.
- **Slave Address:**
  - The master sends a 7-bit address followed by a read/write bit:
    - **0:** Write to the slave.
    - **1:** Read from the slave.
- **Acknowledge (ACK/NACK):**
  - The slave with the corresponding address responds with a bit: ACK (0) if it recognizes the address, or NACK (1) if it does not.
- **Data Transfer:**
  - After the transmission, an ACK is sent to indicate that the byte was correctly received.
- **Stop condition:**
  - The master ends the communication by changing SDA from low to high while SCL is high.
  - This frees the bus for other communications.

## Project Code

### Role Summary

1. **docs/**  
   General documentation, diagrams, manuals.
2. **include/**  
   Headers: function prototypes and constant definitions.
3. **src/**  
   C source code:
   - **i2c.c:** Low-level I2C communication.
   - **mpu.c:** Specific functions to handle the MPU6050 sensor.
   - **main.c:** Entry point and main logic of the program.
4. **Makefile**  
   Defines how the project is compiled and linked.
5. **README.md**  
   Usage guide, compilation instructions, and general project explanation.

## Libraries

- **stdlib.h**  
  Includes functions for memory management, data conversion, and program termination (for example, `exit()`). Although `exit()` is not directly used in this code, it is commonly included for future use.
- **stdint.h**  
  Defines fixed-size integer types, such as `uint8_t` (an unsigned 8-bit integer), which is essential for working with data on the I2C bus.
- **string.h**  
  Provides functions for string and memory manipulation, such as `memcpy()`, which is used to copy data into a buffer.
- **fcntl.h**  
  Contains definitions for file control functions, such as `open()`, which is used to open the I2C device.
- **unistd.h**  
  Offers standard Unix functions, such as `close()`, `read()`, and `write()`, which are necessary for file handling.
- **sys/ioctl.h**  
  Allows performing control operations on devices via the `ioctl()` function. This function is key to configuring the I2C device and sending transactions.
- **linux/i2c-dev.h**  
  Defines structures and constants specific to I2C communication in Linux, such as `struct i2c_msg`, `struct i2c_rdwr_ioctl_data`, the constant `I2C_RDWR` (for combined transactions), and `I2C_SLAVE` (to set the slave device address).

## Functions in *i2c.c*

### i2c_init

1. **Buffer `i2cFile`:**  
   An array of 16 is defined to store the I2C device path.
2. **Construction of the device path:**  
   The path of the file `i2c-1` (which contains the acceleration data) is added to the buffer using `sprintf`.  
   - **What does `sprintf` do?**  
     It formats and stores a series of characters in a string.
3. **Opening the I2C device:**  
   The function calls `open` to open the file in read/write mode. It returns a file descriptor `fd` and checks that no error occurred.  
   - **What is a file descriptor `fd`?**  
     It is an integer that acts as a unique identifier for an open file or device in the operating system. This integer does not contain all the file's information but serves as an index pointing to an internal kernel structure.
4. **Configuring the slave address via `ioctl`:**  
   - **What does `ioctl` do?**  
     It is a Linux system call that allows sending specific commands to a device, performing operations that cannot be done with standard read/write functions. In the context of I2C, it is used to configure the device and execute complex communication transactions.
   - The call `ioctl(fd, I2C_SLAVE, addr);` acts as an initialization to inform the I2C driver that the device we want to communicate with has the address `addr`:
     1. **fd:** The connection to `/dev/i2c-1`.
     2. **I2C_SLAVE:** A constant defined in `linux/i2c-dev.h`.
     3. Once this function is executed, the slave device address is permanently associated with that file descriptor while it remains open, so it is not necessary to set the address again.

```c
int i2c_init(int dev, int addr){
    char i2cFile[16];
    sprintf(i2cFile, "/dev/i2c-%d", dev);
    uint8_t fd = open(i2cFile, O_RDWR);
    if (fd < 0) {
        perror("Error opening the I2C bus");
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        perror("Error configuring I2C_SLAVE address");
        close(fd);
        return -1;
    }
    return fd;
}
```
## i2c_write

1. **How does the buffer work?**  
   The buffer collects the data that needs to be transmitted. In the MPU, the first byte must be the register of the data you want to read; if you want to read the X acceleration, you must specify the corresponding register.

2. **Configuration of structs for the message:**  
   Two structs are configured to create the message.

3. **The function `ioctl(fd, I2C_RDWR, &packets)` does the following:**  
   1. Since the connection (link) has already been established, it already knows which slave it is addressing.  
   2. It uses `I2C_RDWR`.  
   3. Finally, because the `message` structure is correctly formatted according to the I2C frame protocol, the function is able to correctly determine which slave to address, the operation (W/R), and the data to be transmitted.

4. **Message and Packet Structures:**  
   Provided by the `<linux/2c.h>` library:  
   1. `struct i2c_msg`: Represents a transfer message on the I2C bus.  
   2. `struct i2c_rdwr_ioctl_data`: Used with the `ioctl` function to perform multiple read/write operations in a single transaction.

```c
int i2c_write(int fd, uint8_t reg, const uint8_t *data, int len, int addr) {
    // Buffer that contains: [reg][data0][data1]...[dataN]
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    if (len > 0 && data != NULL) {
        memcpy(&buffer[1], data, len);
    }

    // I2C message structure
    struct i2c_msg message;
    struct i2c_rdwr_ioctl_data packets;

    message.addr  = addr;      // Slave address
    message.flags = 0;         // 0 => write
    message.len   = len + 1;     // 1 (register) + N data bytes
    message.buf   = buffer;

    // Package into the transaction structure
    packets.msgs  = &message;
    packets.nmsgs = 1;

    // Call ioctl to perform the transaction
    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Error in i2c_write_data (ioctl I2C_RDWR)");
        return -1;
    }

    return 0;
}
```
### i2c_read

1. **Initial Write Message:**  
   First, a write message is sent that contains the register from which you want to start reading. This informs the slave device which register's data will be requested.

2. **Read Message:**  
   Next, a read message is sent that specifies the number of bytes (`len`) to be read from the designated register.

3. **Grouping the Messages:**  
   Both the write and read messages are grouped together into a single transaction. This allows the I2C driver to handle the complete operation in one call.

4. **Performing the Transaction:**  
   The function uses `ioctl` with the `I2C_RDWR` command to execute the transaction. If the transaction fails, an error is reported. This function is used within *mpu.c* to retrieve sensor data.

```c
int i2c_read(int fd, uint8_t reg, uint8_t *data, int len, int addr) {
    struct i2c_msg messages[2];
    struct i2c_rdwr_ioctl_data packet;
    
    // First message: send the register of interest
    messages[0].addr  = addr;
    messages[0].flags = 0;   // Write mode
    messages[0].len   = 1;
    messages[0].buf   = &reg;
    
    // Second message: read 'len' bytes from that register
    messages[1].addr  = addr;
    messages[1].flags = 1;   // Read mode
    messages[1].len   = len;
    messages[1].buf   = data;
    
    // Group the two messages into a single transaction
    packet.msgs  = messages;
    packet.nmsgs = 2;
    
    // Perform the I2C transaction
    if (ioctl(fd, I2C_RDWR, &packet) < 0) {
        perror("Error in i2c_read_data");
        return -1;
    }
    return 0;
}
```
## mpu.c Functions

### mpu_init

We use the write function to set the MPU register `0x6B` to the value `0x00`, since by default it is “asleep”.

```c
int mpu_init(int fd, int addr){
  uint8_t data = 0x00;    // value to wake up the MPU
  if (i2c_write(fd, DEVICE_RESET, &data, 1, addr)){
    printf("mpu_init error");
    return -1;
  }
  return 0;
}
````
## mpu_read_acceleration

We read the first acceleration register and, starting from that register, define how many bytes to read. Since each acceleration is 16 bits (2 bytes), a total of 6 bytes are read.  
To convert the data from binary to float, it is necessary to “join” the data of each acceleration, as they come separated into bytes (high and low).  
Then, considering that the acceleration values are expressed in terms of g (configured to 2g), they must be divided by the value specified in the datasheet, in this case, 16384.

```c
int mpu_read_acceleration(int fd, int addr, Mpu_accel *accel){
  uint8_t data[6]; // each acceleration occupies 2 bytes

  if (i2c_read(fd, ACCEL_XOUT_H, data, 6, addr)){
    printf("mpu_init error");
    return -1;
  }

  // Combine the high and low bytes
  int16_t raw_x = (data[0] << 8) | data[1];
  int16_t raw_y = (data[2] << 8) | data[3];
  int16_t raw_z = (data[4] << 8) | data[5];

  // Convert from raw to float based on g: 2g
  accel->ax = raw_x / 16384.0;
  accel->ay = raw_y / 16384.0;
  accel->az = raw_z / 16384.0;

  return 0;
}
```
---
Based on: 42Madrid Telefonica, ChatGPT