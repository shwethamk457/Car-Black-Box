/*
 * File:   adc.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 1:59 PM
 */


#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    ADON = 1;   //Turn on ADC
    
//    PCFG0 = 0;
//    PCFG1 = 0;
//    PCFG2 = 0;
//    PCFG3 = 0;  //To select all channels as analog
    
    VCFG0 = 0;
    VCFG1 = 0;  //To select default reference voltage
    
    ADCS0 = 0;
    ADCS1 = 1;
    ADCS2 = 0;  //To select 1TAD = 1.6us
    
    ACQT0 = 0;
    ACQT1 = 1;
    ACQT2 = 0;  //To select 4TAD as acquisition
    
    ADFM = 1;   //To select justification
}

unsigned int read_adc(unsigned char channel)
{
    /*select the channel*/
    ADCON0 = (ADCON0 & 0xC3) | (channel << 2);
    
    /* Start the conversion */
    GO = 1;
    while(GO);
    
    return (ADRESH << 8) | ADRESL;
}