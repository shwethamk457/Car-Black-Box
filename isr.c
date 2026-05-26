/*
 * File:   isr.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:00 PM
 */


#include <xc.h>
#include "timer0.h"

extern int blink_flag;
void __interrupt() isr(void)
{
    // counters used for creating delay (software counters)
    static unsigned int count=0;
    // This block executes whenever Timer0 overflows
    if(TMR0IF == 1)
    {
        TMR0 = TMR0+8;  // reload timer value for desired timing
        if(count++==10000)
        {
            blink_flag = !blink_flag;   //for blink in set time
            count = 0;  //reset counter
      }
      TMR0IF=0; //clear timer0 interrupt flag
    }
}
