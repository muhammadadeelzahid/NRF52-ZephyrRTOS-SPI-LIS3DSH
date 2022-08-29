# NRF52-LIS3DSH
Interfacing LIS3DSH Accelerometer module with NRF52DK (NRF52832) over SPI running Connect SDK using Zephyr's SPI drivers<br />
Uses SPI to communicate with LIS3DSH. <br />
Pins configurations are specified in the overlay file.<br />
Make sure to specify the lis3dsh.c library file in the CMAKELIST.txt for to be included in the build process.<br />



# Requirements
SDK:<br />
nRF Connect SDK v2.0.2<br />

Supported boards:<br />
nrf52dk_nrf52832<br />
nrf52840dk_nrf52840<br />
nrf5340dk_nrf5340_cpuapp<br />
