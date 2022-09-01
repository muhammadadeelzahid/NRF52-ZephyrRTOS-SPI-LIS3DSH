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
	
    //Call the SPI initialization function
    SPI_Init();
    
	int acc_x, acc_y, acc_z ;
	//values that hold the values of acceleration in mg
    while(true)
      {
		get_acceleration(&acc_x,&acc_y,&acc_z);		
		printk("X= %6dmg Y= %6dmg Z= %6dmg \r\n", acc_x, acc_y, acc_z);
		k_msleep(300);
      }

}
