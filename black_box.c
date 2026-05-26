/*
 * File:   black_box.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:03 PM
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

extern unsigned int prev_speed, cur_speed;
extern unsigned int prev_gear_position;
extern int cur_gear_position;
extern unsigned int flag, flag1;
extern unsigned int event_index;
unsigned char switch_value;
int menu_index;
char *menu[] = {"View Log", "Clear Log", "Download Log", "Set Time"};

void process_vehicle_data() 
{
    switch_value = read_matrix_keypad(STATE_CHANGE);
    if (flag == 0) 
    {
        /*************DASHBOARD************************************/
        static int once = 0;
        if(once == 0)
        {
            CLEAR_DISP_SCREEN;
            once = 1;
        }
        //CLEAR_DISP_SCREEN;
        cur_speed = read_adc(CHANNEL4)/10.23;
        if (switch_value == SWITCH1)
        {
            if (cur_gear_position < 6)
                cur_gear_position++;
        } 
        else if (switch_value == SWITCH2) 
        {
            if (cur_gear_position > 0)
                cur_gear_position--;
        }
        else if (switch_value == SWITCH3) 
        {
            cur_gear_position = 7;
        }

        if ((prev_gear_position != cur_gear_position)) 
        {
            if (cur_gear_position >= 0) 
            {
                event_index++;
                prev_speed = cur_speed;
                prev_gear_position = cur_gear_position;
                store_data();
            }
        }
        print_dashboard();
        if (switch_value == SWITCH11) 
        {
            flag = 1;
            once = 0;
            CLEAR_DISP_SCREEN;
        }
    } 
    else if (flag == 1) 
    {
        /**********MENU SCREEN*************************/
        if (menu_index == 1) 
        {
            view_log();
        } 
        else if (menu_index == 2) 
        {
            clear_log();
        } 
        else if (menu_index == 3) 
        {
            download_log();
        } 
        else if (menu_index == 4) 
        {
            set_time();
        } 
        else 
        {
            if (switch_value == SWITCH11) 
            {
                menu_index = flag1;
            }
            if (switch_value == SWITCH1) 
            {
                if (flag1 < 4)
                    flag1++;
            }
            else if (switch_value == SWITCH2) 
            {
                if (flag1 > 1)
                    flag1--;
            }
            else if(switch_value == SWITCH12)
            {
                if(flag1 > 1)
                {
                    flag1 = 1;
                    menu_index = 0;
                }
                else
                {
                    flag = 0;
                }
            }
            /********MENU SCREEN PRINTING****************/
            if(flag1 == 1)
            {
                clcd_print("*",LINE1(0));
                clcd_print(" ",LINE2(0));
                clcd_print(menu[flag1-1],LINE1(1));
                clcd_print(menu[flag1],LINE2(1));
            }
            else
            {
                clcd_print(" ",LINE1(0));
                clcd_print("*",LINE2(0));
                clcd_print(menu[flag1-2],LINE1(1));
                clcd_print(menu[flag1-1],LINE2(1));
            }
        }
    }
}
