
#include "lis3dsh.h"

#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/SPi.h>




#define SPI_BUFSIZE  8   //SPI Communication buffer size
#define SPI_INSTANCE  0 //SPI Instance to be used

uint8_t   spi_tx_buf[SPI_BUFSIZE]; // spi tx buffer 
uint8_t   spi_rx_buf[SPI_BUFSIZE]; // spi rx buffer

//spi_buf and spi_buf_set structures to hold the buffer values for SPI
struct spi_buf tx_buf_arr; 
struct spi_buf_set tx;
struct spi_buf rx_buf_arr;
struct spi_buf_set rx ;

volatile  uint8_t   SPIReadLength, SPIWriteLength; // variables to hold read and write lengths



const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi1)) ;
struct spi_config spi_cfg ;
struct spi_cs_control chip;

/* A function to convert two's complement value to 16-bit int value */
int twoComplToInt16(int twoComplValue)
{
	  int int16Value = 0;

	  /* conversion */
	  if(twoComplValue > 32768)
	  {
		    int16Value = -(((~twoComplValue) & 0xFFFF) + 1);
	  }
	  else
	  {
		    int16Value = twoComplValue;
	  }

	  return int16Value;
}




/* A function to write into LIS3DSH Internal Register */
static void LIS3DSH_write_reg(int reg, int data)
{
    SPIWriteLength = 2; // set the spi write length to 2 bytes
    SPIReadLength = 0; // set the read length
    
    spi_tx_buf[0] = SET_WRITE_SINGLE_CMD(reg); // set the first byte which is a write command
    spi_tx_buf[1] = data; // A byte of data to be sent
	

   //specifying the tx and rx buffer specific to zephyr's SPI drivers
   	tx_buf_arr.buf = spi_tx_buf;
    tx_buf_arr.len = SPIWriteLength;
    tx.buffers = &tx_buf_arr;
    tx.count = 1;
	
    rx_buf_arr.buf = spi_rx_buf;
    rx_buf_arr.len = SPIReadLength;

    rx.buffers = &rx_buf_arr ;
    rx.count = 1;

	int error = spi_transceive(spi_dev, &spi_cfg, &tx,&rx);
	if(error != 0){
		printk("SPI transceive error: %i\n", error);
	}

    
}




/* A function to read a value from the internal register of lis3dsh */
int LIS3DSH_read_reg(int reg)
{	
    /* Set the read command for reading a single byte */
    spi_tx_buf[0] = SET_READ_SINGLE_CMD(reg);
  


   //specifying the tx and rx buffer specific to zephyr's SPI drivers
   	tx_buf_arr.buf = spi_tx_buf;
    tx_buf_arr.len = 2;

    tx.buffers = &tx_buf_arr;
    tx.count = 1;
	
    rx_buf_arr.buf = spi_rx_buf;
    rx_buf_arr.len = 2;

    rx.buffers = &rx_buf_arr ;
    rx.count = 1;

	int error = spi_transceive(spi_dev,&spi_cfg,&tx,&rx);
	if(error != 0){
		printk("SPI transceive error: %i\n", error);
		return error;
	}

    return spi_rx_buf[1];
}

void printbuffers()
{
    printk("SPI TX: ");
	for (int j = 0; j<sizeof(spi_tx_buf); j++)
	{
		printk(" %x",spi_tx_buf[j]);
	}
	printk("\r\nSPI RX: ");
	for (int j = 0; j<sizeof(spi_rx_buf); j++)
	{
		printk(" %x",spi_rx_buf[j]);
	}
    printk("\r\n");

}



/* A function to initialize SPI Instance */
void SPI_Init(void)
{
 	if (!device_is_ready(spi_dev)) {
        /* Device is not ready to use */
		printk("\r\nStop device not ready\r\n");
	}
    
	chip.gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    chip.gpio_pin = 17;
    chip.gpio_dt_flags = GPIO_ACTIVE_LOW;
    chip.delay = 2;
	if (chip.gpio.pin == NULL)
		printk("CS CHIP PIN NOT READY\r\n");
	

    

    spi_cfg.frequency = 4000000;
    spi_cfg.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
				 SPI_MODE_CPOL | SPI_MODE_CPHA;
    spi_cfg.slave = 0;
    spi_cfg.cs = &chip;


    LIS3DSH_init();
}



/* A function to initialize the Lis3dsh Sensor */
void LIS3DSH_init(void)
{
    
    int regval;

    /* Read the who am i register to confirm lis3dsh is communicating with our nrf device */
    regval = LIS3DSH_read_reg(ADD_REG_WHO_AM_I);

    /* check the who am i register value and compare it with default who am i value for lis3dsh 
        if who am i is correct then proced */
    if( regval == UC_WHO_AM_I_DEFAULT_VALUE )
    {
        LIS3DSH_write_reg(ADD_REG_CTRL_4, UC_ADD_REG_CTRL_4_CFG_VALUE);

            /* verify written value */
        regval = LIS3DSH_read_reg(ADD_REG_CTRL_4);

            /* if written value is different */
        if( regval != UC_ADD_REG_CTRL_4_CFG_VALUE )
        {
                /* ERROR: stay here... */
            while(1)
            {
                printk("\r\nWrite Reg ERR\r\n");
                k_msleep(500);
            }
        }
    }
    else
    {
      /* ERROR: stay here... */
      while(1)
        {
            printk("\nDevice does not exist\r\n");
            k_msleep(1000000);
        }			   
    }
}

void get_acceleration( int* x, int* y, int* z)
{

    *x = ((LIS3DSH_read_reg(ADD_REG_OUT_X_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_X_L));
    *y = ((LIS3DSH_read_reg(ADD_REG_OUT_Y_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Y_L));
    *z = ((LIS3DSH_read_reg(ADD_REG_OUT_Z_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Z_L));

    /* transform X value from two's complement to 16-bit int */
    *x = twoComplToInt16(*x);
    /* convert X absolute value to mg value */
    *x = *x * SENS_2G_RANGE_MG_PER_DIGIT;

    /* transform Y value from two's complement to 16-bit int */
    *y = twoComplToInt16(*y);
    /* convert Y absolute value to mg value */
    *y = *y * SENS_2G_RANGE_MG_PER_DIGIT;

    /* transform Z value from two's complement to 16-bit int */
    *z = twoComplToInt16(*z);
    /* convert Z absolute value to mg value */
    *z = *z * SENS_2G_RANGE_MG_PER_DIGIT;

    
}