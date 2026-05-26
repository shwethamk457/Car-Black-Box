/*
 * File:   timer0.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:00 PM
 */


#include <xc.h>
#include "timer0.h"

void init_timer0(void)
{
    TMR0ON = 1; //turn on the timer
    
    T08BIT = 1; //select the 8 bit timer
    
    T0CS = 0; //select the internal clock source
    
    PSA = 1; //pre scaler not used
    
    //Configure the timer as interrupt
    GIE = 1;
    
    PEIE=1;
    
    TMR0IE = 1;
    
    TMR0IF = 0;
    
    TMR0 = 6;
    
}
