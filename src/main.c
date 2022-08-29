/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/SPi.h>
#include "lis3dsh.h"


void main(void)
{



    // print the log msg over uart port
    printk("This is log data from nordic device..\r\n");


    // variables to hold x y z values in mg
    int intValueMgX, intValueMgY, intValueMgZ;
	
	
	
    //Call the SPI initialization function
    SPI_Init();
    
    // Call the Lis3dsh initialization function
    LIS3DSH_init();


    while(true)
      {
		//
		intValueMgX = ((LIS3DSH_read_reg(ADD_REG_OUT_X_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_X_L));
		intValueMgY = ((LIS3DSH_read_reg(ADD_REG_OUT_Y_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Y_L));
		intValueMgZ = ((LIS3DSH_read_reg(ADD_REG_OUT_Z_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Z_L));

		/* transform X value from two's complement to 16-bit int */
		intValueMgX = twoComplToInt16(intValueMgX);
		/* convert X absolute value to mg value */
		intValueMgX = intValueMgX * SENS_2G_RANGE_MG_PER_DIGIT;

		/* transform Y value from two's complement to 16-bit int */
		intValueMgY = twoComplToInt16(intValueMgY);
		/* convert Y absolute value to mg value */
		intValueMgY = intValueMgY * SENS_2G_RANGE_MG_PER_DIGIT;

		/* transform Z value from two's complement to 16-bit int */
		intValueMgZ = twoComplToInt16(intValueMgZ);
		/* convert Z absolute value to mg value */
		intValueMgZ = intValueMgZ * SENS_2G_RANGE_MG_PER_DIGIT;
                //
		printk("X=%6d Y=%6d Z=%6d \r\n", intValueMgX, intValueMgY, intValueMgZ);
		k_msleep(300);
      }


}
