#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR 0x40

int main (void) {
    char buffer[1];
    char humidity_result[2];
    char temperature_result[2];
    float humidity = 0, temperature = 0;
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
    sleep(0.25);
    read(fd, humidity_result, 2);
    printf("0x%02X 0x%02X \n", humidity_result[0], humidity_result[1]);
    humidity = ((((humidity_result[0] * 256 + humidity_result[1]) * 125.0) / 65536.0) - 6.0);
    printf("%.2lf%%\n", humidity);
    
    
    buffer[0]=0xE3;
    write(fd, buffer, 1);
    sleep(0.25);
    read(fd, temperature_result, 2);
    printf("0x%02X 0x%02X \n", temperature_result[0], temperature_result[1]);
    temperature = ((((temperature_result[0] * 256 + temperature_result[1]) * 175.72) / 65536.0) - 46.85);
    printf("%.2lfC\n", temperature);
    
    return 0;
}
