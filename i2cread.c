#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR 0x40

int main (void) {
    char buffer[1];
    char result[2];
    double humidity = 0;
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

    read(fd, result, 2);
    printf("0x%02X 0x%02X \n", result[0], result[1]);
    humidity = (double) ((((result[0] * 256.0 + result[1]) * 125.0) / 65536.0) - 6.0);
    printf("%lf%%\n", humidity);
    return 0;
}
