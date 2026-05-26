/*
 * File:   EEPROM.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:00 PM
 */


#include <xc.h>
#include "EEPROM.h"

void write_internal_eeprom(unsigned char address, unsigned char data)
{
    EEADR = address;
    EEDATA = data;
    
    EEPGD = 0;
    CFGS = 0;
    WREN = 1;
    GIE = 0;
    
    EECON2 = 0x55;
    EECON2 = 0xAA;
    
    WR = 1;
    GIE = 1;
    
    while(!EEIF);
    
    EEIF = 0;
    WREN = 0;
    
}

unsigned char read_internal_eeprom(unsigned char address)
{
    EEADR = address;
    
    EEPGD = 0;
    
    CFGS = 0;
    
    RD = 1;
    
    return EEDATA;
}