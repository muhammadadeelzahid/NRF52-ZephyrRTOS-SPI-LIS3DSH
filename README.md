# NRF52-LIS3DSH
Interfacing LIS3DSH Accelerometer module with NRF52DK (NRF52832) over SPI running Connect SDK using Zephyr's SPI drivers
Uses SPI instance 1 to communicate with LIS3DSH specified in the prj.conf file. 
Pins configurations are specified in the overlay file.
Make sure to specify the lis3dsh.c file in the CMAKELIST.txt for to be included in the build process.



# Requirements
SDK:

nRF Connect SDK v2.0.2

Supported boards:
nrf52dk_nrf52832
nrf52840dk_nrf52840
nrf5340dk_nrf5340_cpuapp
