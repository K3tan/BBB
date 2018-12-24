// Code to measure temperature and humidity from Si7021 breakout board on BeagleBone black.

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main()
{
    char regAddress[] = {0xE5, 0xE0};
    char humidityCode[2] = {0};
    char temperatureCode[2] = {0};
    float humidityValue = 0, temperatureValue = 0; 


	// Create I2C bus
	int i2cBus;
	char *bus = "/dev/i2c-2";
	if((i2cBus = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}

    ioctl(i2cBus, I2C_SLAVE, 0x40);

    // Send humidity measurement command(0xF5)
	write(i2cBus, regAddress[0], 1);
	sleep(1);

	// Read 2 bytes of humidity data
	if(read(i2cBus, humidityCode, 2) != 2)
	{
		printf("Error : Input/output Error \n");
	}
	else
	{
		// Convert the data
		humidityValue = (((humidityCode[0] * 256 + humidityCode[1]) * 125.0) / 65536.0) - 6;

		// Output data to screen
		printf("Relative Humidity : %.2f RH \n", humidityValue);
	}

}
