//  TEMPERATURE CONTROL UNIT
//  CODE WRITTEN BY:
//  KETAN KARAMBELKAR & JAGRUTI KAPGATE


#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

// Define 7-bit address of i2c device
#define I2C_ADDR 0x40

int main (void) 
{
    char buffer[1];     //command buffer
    char humidity_result[2];    //register value of Si7021 sensor for humidity    
    char temperature_result[2];     //register value of Si7021 sensor for temperature
    float humidity = 0, temperature = 0;    //final results
    int fd;     //file

    fd = open("/dev/i2c-2", O_RDWR);    //open i2c bus
    
    //check for bus opening errors
    if (fd < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }
    
    //ioctl
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }
    
    //infinite loop for continuous monitoring
    while(1)
    {
        buffer[0]=0xE5;     //send humidity measurement command
        write(fd, buffer, 1);
        sleep(0.25);
        read(fd, humidity_result, 2);      //read two bytes
        //printf("0x%02X 0x%02X \n", humidity_result[0], humidity_result[1]);   //for debugging
        //calculate humidity
        humidity = ((((humidity_result[0] * 256 + humidity_result[1]) * 125.0) / 65536.0) - 6.0);   
        printf("%.2lf%%\n", humidity);
    
        buffer[0]=0xE3;     //send temperature measurement command
        write(fd, buffer, 1);
        sleep(0.25);
        read(fd, temperature_result, 2);    //read two bytes
        //printf("0x%02X 0x%02X \n", temperature_result[0], temperature_result[1]);     //for debugging
        //calculate temperature
        temperature = ((((temperature_result[0] * 256 + temperature_result[1]) * 175.72) / 65536.0) - 46.85);
        printf("%.2lfC\n", temperature);
        
        //control section of TCU
        if(temperature < 5)
        {
            // TURN HEATER ON
        }
        else if(temperature > 30)
        {
            // TURN HEATER OFF
        }    
        else if(humidity > 50 && temperature < 30)
        {
            // TURN HEATER ON
        }    
        else if(humidity > 90)
        {
            // TURN HEATER ON & OPC-N3 OFF
        }
        else if(temperature < 0)
        {
            // TURN OPC-N3 OFF
        }
        
        
    }
    return 0;
}
