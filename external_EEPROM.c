/*
 * File:   external_EEPROM.c
 * Author: Shwetha M K
 *
 * Created on 16 May, 2026, 8:50 PM
 */


#include <xc.h>
#include "i2c.h"
#include "external_EEPROM.h"

void write_AT24C04(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();  
	for(unsigned int i = 1000; i--;);
}

unsigned char read_AT24C04(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}