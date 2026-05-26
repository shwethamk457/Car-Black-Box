/* 
 * File:   external_EEPROM.h
 * Author: Shwetha M K
 *
 * Created on 16 May, 2026, 8:50 PM
 */

#ifndef EXTERNAL_EEPROM_H
#define	EXTERNAL_EEPROM_H

#define SLAVE_READ		0xA1
#define SLAVE_WRITE		0xA0

#define DAY_ADDR		0x03
#define DATE_ADDR		0x04
#define MONTH_ADDR      0x05
#define YEAR_ADDR		0x06
#define CNTL_ADDR		0x07

void write_AT24C04(unsigned char address1,  unsigned char data);
unsigned char read_AT24C04(unsigned char address1);

#endif	/* EXTERNAL_EEPROM_H */

