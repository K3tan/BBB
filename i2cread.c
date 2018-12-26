#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR 0x40

int main (void) {
    char buffer[2];
    int fd;

    fd = open("/dev/i2c-2", O_RDWR);

    if (fd < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }

    buffer[0]=0xE5;
    write(fd, buffer, 1);

    read(fd, buffer, 2);
    printf("0x%02X 0x%02X \n", buffer[0], buffer[1]);
    return 0;
}
