/* 
 * File:   EEPROM.h
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 1:57 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

void write_internal_eeprom(unsigned char address, unsigned char data);
unsigned char read_internal_eeprom(unsigned char address);


#endif	/* EEPROM_H */

