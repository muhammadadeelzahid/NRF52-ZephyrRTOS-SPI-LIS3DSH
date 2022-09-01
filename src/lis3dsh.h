#ifndef LIS3DSH_H
#define LIS3DSH_H


/* Pins Definition for lis3dsh */
#define SPI_SS_PIN   25  // pin to be connected to cs(chip select pin)
#define SPI_SCK_PIN  22  // pin to be connected to clock pin (scl)
#define SPI_MISO_PIN 24  // pin to be connected to MISO 
#define SPI_MOSI_PIN 23  // pin to be connected to MOSI


#define    SPIFlash_CS_LOW    nrf_gpio_pin_clear(SPI_SS_PIN)   // a simple function call to clear the cs pin
#define    SPIFlash_CS_HIGH   nrf_gpio_pin_set(SPI_SS_PIN)     //a simple function call to set the cs pin 


/* LIS3DSH registers addresses */
#define ADD_REG_WHO_AM_I				0x0F
#define ADD_REG_CTRL_4					0x20
#define ADD_REG_OUT_X_L					0x28
#define ADD_REG_OUT_X_H					0x29
#define ADD_REG_OUT_Y_L					0x2A
#define ADD_REG_OUT_Y_H					0x2B
#define ADD_REG_OUT_Z_L					0x2C
#define ADD_REG_OUT_Z_H					0x2D

/* WHO AM I register default value */
#define UC_WHO_AM_I_DEFAULT_VALUE		0x3F

/* ADD_REG_CTRL_4 register configuration value: X,Y,Z axis enabled and 400Hz of output data rate
   for more info see the datasheet of Lis3dsh
 */
#define UC_ADD_REG_CTRL_4_CFG_VALUE		0x77

/* Sensitivity for 2G range [mg/digit] */
#define SENS_2G_RANGE_MG_PER_DIGIT		((float)0.06)

/* LED threshold value in mg */
#define LED_TH_MG						(1000)	/* 1000mg (1G) */


/* ---------------- Local Macros ----------------- */

/* set read single command. Attention: command must be 0x3F at most */
#define SET_READ_SINGLE_CMD(x)			(x | 0x80)
/* set read multiple command. Attention: command must be 0x3F at most */
#define SET_READ_MULTI_CMD(x)			(x | 0xC0)
/* set write single command. Attention: command must be 0x3F at most */
#define SET_WRITE_SINGLE_CMD(x)			(x & (~(0xC0)))
/* set write multiple command. Attention: command must be 0x3F at most */
#define SET_WRITE_MULTI_CMD(x)			(x & (~(0x80))	\
                                                 x |= 0x40)

void SPI_Init(void);                      /* A function to intialize the SPI communication */
void LIS3DSH_init(void);                  /* Funtiont to initialize LIS3DSH sensor */
int twoComplToInt16(int twoComplValue);   /* A function to convert 2's complement values to int16 values */
int LIS3DSH_read_reg(int reg);            /* A function to read a value from a register */
void printbuffers();
void get_acceleration(int*,int*,int*);

#endif
/********************************************END FILE*******************************************/
