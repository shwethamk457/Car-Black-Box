/*
 * File:   matrix_keypad.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:00 PM
 */


#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad()
{
    TRISB = (TRISB & 0x1F) | 0x1E;
    
    RBPU = 0;//To enable pull-up ckt
    
}

unsigned char scan_key(void)
{
    ROW0 = 0;
    ROW1 = 1;
    ROW2 = 1;
    
    if(COL0 == 0)
    {
        return SWITCH1;
    }
    else if(COL1 == 0)
    {
        return SWITCH4;
    }
    else if(COL2 == 0)
    {
        return SWITCH7;
    }
    else if(COL3 == 0)
    {
        return SWITCH10;
    }
    
    ROW0 = 1;
    ROW1 = 0;
    ROW2 = 1;
    if(COL0 == 0)
    {
        return SWITCH2;
    }
    else if(COL1 == 0)
    {
        return SWITCH5;
    }
    else if(COL2 == 0)
    {
        return SWITCH8;
    }
    else if(COL3 == 0)
    {
        return SWITCH11;
    }
    
    ROW0 = 1;
    ROW1 = 1;
    ROW2 = 0;
    ROW2 = 0;
    if(COL0 == 0)
    {
        return SWITCH3;
    }
    else if(COL1 == 0)
    {
        return SWITCH6;
    }
    else if(COL2 == 0)
    {
        return SWITCH9;
    }
    else if(COL3 == 0)
    {
        return SWITCH12;
    }
    
    return ALL_RELEASED;
}

unsigned char read_matrix_keypad(unsigned char trigger)
{
    unsigned static int once = 1;
    if(trigger == LEVEL)
    {
        return scan_key();
    }
    else if(trigger == STATE_CHANGE)
    {
        unsigned char key = scan_key();
        if((key != ALL_RELEASED) && (once == 1))
        {
            once = 0;
            return key;
        }
        else if(key == ALL_RELEASED)
        {
            once = 1;
        }
    }
    return ALL_RELEASED;
}
