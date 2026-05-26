/*
 * File:   main.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:00 PM
 */


#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "EEPROM.h"
#include "timer0.h"
#include "external_EEPROM.h"
#include "matrix_keypad.h"
#include "message_handler.h"
#include "uart.h"

unsigned int prev_speed = 0, cur_speed;
unsigned int prev_gear_position = 0;
int cur_gear_position = -1;
unsigned int flag = 0;
unsigned int flag1 = 1;
unsigned int event_index = 0;

void main(void) 
{
    init_adc();
    init_clcd();
    init_i2c();
    init_ds1307();
    init_matrix_keypad();
    init_uart();
    init_timer0();
    //event_index = read_internal_eeprom(0x7F);
    event_index = read_AT24C04(0x7F);
    while(1)
    {
        //function call to process
        process_vehicle_data();
    }
    return;
}
