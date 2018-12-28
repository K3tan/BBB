//  TEMPERATURE CONTROL UNIT
//  CODE WRITTEN BY:
//  KETAN KARAMBELKAR & JAGRUTI KAPGATE


#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

// Define 7-bit addresses of i2c devices
#define SI7021_ADDR 0x40
#define ADS1115_ADDR 0x48       //when you connect ADDR pin to GND

int16_t adc_driver();

int main (void) 
{
    char buffer[1];     //command buffer
    char humidity_result[2];    //register value of Si7021 sensor for humidity    
    char temperature_result[2];     //register value of Si7021 sensor for temperature
    float humidity = 0, temperature = 0;    //final results
    int I2C_file1;     //file
    int16_t flowRate = 0;

    I2C_file1 = open("/dev/i2c-2", O_RDWR);    //open i2c bus
    
    //check for bus opening errors
    if (I2C_file1 < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }
    
    //ioctl
    if (ioctl(I2C_file1, I2C_SLAVE, SI7021_ADDR) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }
    
    //infinite loop for continuous monitoring
    while(1)
    {
        buffer[0]=0xE5;     //send humidity measurement command
        write(I2C_file1, buffer, 1);
        sleep(0.25);
        read(I2C_file1, humidity_result, 2);      //read two bytes
        //printf("0x%02X 0x%02X \n", humidity_result[0], humidity_result[1]);   //for debugging
        //calculate humidity
        humidity = ((((humidity_result[0] * 256 + humidity_result[1]) * 125.0) / 65536.0) - 6.0);   
        printf("%.2lf%%\n", humidity);
    
        buffer[0]=0xE3;     //send temperature measurement command
        write(I2C_file1, buffer, 1);
        sleep(0.25);
        read(I2C_file1, temperature_result, 2);    //read two bytes
        //printf("0x%02X 0x%02X \n", temperature_result[0], temperature_result[1]);     //for debugging
        //calculate temperature
        temperature = ((((temperature_result[0] * 256 + temperature_result[1]) * 175.72) / 65536.0) - 46.85);
        printf("%.2lfC\n", temperature);

        //flow rate
        flowRate = adc_driver();
        printf("Current flow = %f SLPM", (float)flowRate*4.096/32767.0);
        
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

int16_t adc_driver()
{
    int writeBuf[3];		//write buffer
    int readBuf[2];		    //read buffer
  
    int16_t val;				//stores the 16 bit value of our ADC conversion

    writeBuf[0] = 1;			//this sets the pointer register so that the following two bytes write to the config register
    writeBuf[1] = 0xC3;   	    //11000011
    writeBuf[2] = 0x03;         //00000011

    //initialize the read buffer to 0
    readBuf[0]= 0;		
    readBuf[1]= 0;

    int I2C_file2;
    I2C_file2 = open("/dev/i2c-2", O_RDWR);    //open i2c bus   

    //check for bus opening errors
    if (I2C_file2 < 0) {
        printf("Error opening file: %s\n", strerror(errno));
        return 1;
    }
    
    //ioctl
    if (ioctl(I2C_file2, I2C_SLAVE, ADS1115_ADDR) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        return 1;
    }

    write(I2C_file2, writeBuf, 3);
    sleep(0.25);

    //wait for the conversion to complete
    while ((readBuf[0] & 0x80) == 0)	//readBuf[0] contains 8 MSBs of config register, AND with 10000000 to select bit 15
    {
	    read(I2C_file2, readBuf, 2);	// Read the config register into readBuf
    }

    writeBuf[0] = 0;					//set pointer register to 0 to read from the conversion register
    write(I2CFile, writeBuf, 1);

    read(I2C_file2, readBuf, 2);          //read 2 bytes into read buffer

    val = (readBuf[0] << 8) + readBuf[1];

    close(I2C_file2);
    return val;
}
