/*
 * File:   menu_data.c
 * Author: Shwetha M K
 *
 * Created on 14 May, 2026, 2:09 PM
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
extern unsigned int flag1,flag;
extern unsigned int event_index;
extern char *menu[];
extern int menu_index;
extern unsigned char switch_value;
char *gear[] = {"GN","G1","G2","G3","G4","G5","GR","COL"};
int blink_flag;

unsigned char clock_reg[3];
unsigned char time[9];

void display_time(void)
{
	clcd_print(time, LINE2(6));

	if (clock_reg[0] & 0x40)
	{
		if (clock_reg[0] & 0x20)
		{
			clcd_print("PM", LINE2(14));
		}
		else
		{
			clcd_print("AM", LINE2(14));
		}
	}
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void store_data()
{
    unsigned char address = ((event_index - 1) % 10) * 12;
    //CLEAR_DISP_SCREEN;
    //write_internal_eeprom(0x7F,event_index);
    write_AT24C04(0x7F,event_index);
    //__delay_ms(5);
    /*Store speed*/
//    write_internal_eeprom(address,(cur_speed/10));
//    write_internal_eeprom(address+1,(cur_speed%10));
        __delay_ms(10);

    write_AT24C04(address,(cur_speed/10) + '0');
    __delay_ms(5);
    write_AT24C04(address+1,(cur_speed%10 ) + '0');
    __delay_ms(5);
    
    /*Store gear*/
//    write_internal_eeprom(address+2,gear[cur_gear_position][0]);
//    write_internal_eeprom(address+3,gear[cur_gear_position][1]);
    write_AT24C04(address+2,gear[cur_gear_position][0]);
    __delay_ms(5);
    write_AT24C04(address+3,gear[cur_gear_position][1]);
    __delay_ms(5);
    /*Store TIME*/
    get_time();

    write_AT24C04(address+4,time[0]);
    __delay_ms(5);
    write_AT24C04(address+5,time[1]);
    __delay_ms(5);
    write_AT24C04(address+6,time[2]);
    __delay_ms(5);
    write_AT24C04(address+7,time[3]);
    __delay_ms(5);
    write_AT24C04(address+8,time[4]);
    __delay_ms(5);
    write_AT24C04(address+9,time[5]);
    __delay_ms(5);
    write_AT24C04(address+10,time[6]);
    __delay_ms(5);
    write_AT24C04(address+11,time[7]);
    __delay_ms(5);    
//    write_internal_eeprom(address+4,time[0]);
//    write_internal_eeprom(address+5,time[1]);
//    write_internal_eeprom(address+6,time[2]);
//    write_internal_eeprom(address+7,time[3]);
//    write_internal_eeprom(address+8,time[4]);
//    write_internal_eeprom(address+9,time[5]);
//    write_internal_eeprom(address+10,time[6]);
//    write_internal_eeprom(address+11,time[7]);
    
}

void print_dashboard()
{
    //CLEAR_DISP_SCREEN;
    clcd_print("SD",LINE1(0));
    clcd_print("GR",LINE1(3));
    clcd_print("TIME",LINE1(7));
    
    clcd_putch((cur_speed/10) + '0',LINE2(0));
    clcd_putch((cur_speed%10) + '0',LINE2(1));
    if(cur_gear_position < 0)
    {
        clcd_print("ON",LINE2(3));
    }
    else
    {
        clcd_print(gear[cur_gear_position],LINE2(3));
    }
    
    get_time();
    display_time();
}

void view_log()
{
    //print the stored data in clcd
    static unsigned int first_entry = 1;
    if(event_index == 0)
    {
        if(first_entry)
        {
            CLEAR_DISP_SCREEN;
            first_entry = 0;
        }
        clcd_print("NO LOG",LINE1(4));
        if(switch_value == SWITCH12)
        {
            menu_index = 0;
            first_entry = 1;
            flag1 = 1;
            return;
        }
    }
    else
    {
        unsigned char address = 0;
        static unsigned int i = 0;
        unsigned int total_logs;
        unsigned int start_index;
        unsigned int actual_index;
        if(first_entry)
        {
            CLEAR_DISP_SCREEN;
            first_entry = 0;
        }
        if(event_index < 10)
        {
            total_logs = event_index;
        }
        else
        {
            total_logs = 10;
        }
        
        if(switch_value == SWITCH1)
        {
            if(i < (total_logs-1))
                i++;
        }
        else if(switch_value == SWITCH2)
        {
            if(i > 0)
                i--;
        }
        else if(switch_value == SWITCH12)
        {
            i = 0;
            first_entry = 1;
            menu_index = 0;
            flag1 = 1;
            CLEAR_DISP_SCREEN;
            return;
        }
        
        if(event_index < 10)
            start_index = 0;
        else
            start_index = event_index % 10;
        
        actual_index = (start_index + i) % 10;
        
        address = actual_index * 12;
        
        clcd_print("SP",LINE1(2));
        clcd_print("GR",LINE1(5));
        clcd_print("TIME",LINE1(8));
        
        clcd_putch('#',LINE1(0));
        clcd_putch(i+'0',LINE2(0));
        
        /*SPEED*/
        //clcd_putch(read_internal_eeprom(address + 0)+'0',LINE2(2));

        //clcd_putch(read_internal_eeprom(address + 1)+'0',LINE2(3));

        clcd_putch(read_AT24C04(address) ,LINE2(2));
        clcd_putch(read_AT24C04(address + 1),LINE2(3));
            
        /*GEAR*/
//        clcd_putch(read_internal_eeprom(address + 2),LINE2(5));
//        clcd_putch(read_internal_eeprom(address + 3),LINE2(6));
        clcd_putch(read_AT24C04(address + 2),LINE2(5));
        clcd_putch(read_AT24C04(address + 3),LINE2(6));
            
        /*TIME*/
//        clcd_putch(read_internal_eeprom(address + 4),LINE2(8));
//        clcd_putch(read_internal_eeprom(address + 5),LINE2(9));
//        clcd_putch(read_internal_eeprom(address + 6),LINE2(10));
//        clcd_putch(read_internal_eeprom(address + 7),LINE2(11));
//        clcd_putch(read_internal_eeprom(address + 8),LINE2(12));
//        clcd_putch(read_internal_eeprom(address + 9),LINE2(13)); 
//        clcd_putch(read_internal_eeprom(address + 10),LINE2(14));
//        clcd_putch(read_internal_eeprom(address + 11),LINE2(15));
        clcd_putch(read_AT24C04(address + 4),LINE2(8));
        clcd_putch(read_AT24C04(address + 5),LINE2(9));
        clcd_putch(read_AT24C04(address + 6),LINE2(10));
        clcd_putch(read_AT24C04(address + 7),LINE2(11));
        clcd_putch(read_AT24C04(address + 8),LINE2(12));
        clcd_putch(read_AT24C04(address + 9),LINE2(13));
        clcd_putch(read_AT24C04(address + 10),LINE2(14));
        clcd_putch(read_AT24C04(address + 11),LINE2(15));
    }
}

void clear_log()
{
    event_index = 0;
    //write_internal_eeprom(0x7F,0);
    write_AT24C04(0x7F,0);
    CLEAR_DISP_SCREEN;
    clcd_print("LOG CLEAED!",LINE1(2));
    __delay_ms(1000);
    CLEAR_DISP_SCREEN;
    menu_index = 0;
    flag1 = 1;  
}

void download_log()
{
    if(event_index == 0)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("NO LOG",LINE1(6));
    }
    else
    {
        int run_index = 0;
        unsigned char address;
        if(event_index < 10)
        {
            run_index = event_index;
        }
        else
        {
            run_index = 10;
        }
        for(int i = 0; i < run_index; i++)
        {
            address = (((event_index - 1 + i) % 10)) * 12;
            /*TIME*/
//            putch(read_internal_eeprom(address + 4));
//            putch(read_internal_eeprom(address + 5));
//            putch(read_internal_eeprom(address + 6));
//            putch(read_internal_eeprom(address + 7));
//            putch(read_internal_eeprom(address + 8));
//            putch(read_internal_eeprom(address + 9));
//            putch(read_internal_eeprom(address + 10));
//            putch(read_internal_eeprom(address + 11));
            putch(read_AT24C04(address + 4));
            putch(read_AT24C04(address + 5));
            putch(read_AT24C04(address + 6));
            putch(read_AT24C04(address + 7));
            putch(read_AT24C04(address + 8));
            putch(read_AT24C04(address + 9));
            putch(read_AT24C04(address + 10));
            putch(read_AT24C04(address + 11));
            putch('_');

            /*GEAR*/
//            putch(read_internal_eeprom(address + 2));
//            putch(read_internal_eeprom(address + 3));
            putch(read_AT24C04(address + 2));
            putch(read_AT24C04(address + 3));
            putch('_');

            /*SPEED*/
//            putch((read_internal_eeprom(address + 0)) + '0');
//            putch((read_internal_eeprom(address + 1)) + '0');
            putch(read_AT24C04(address + 0));
            putch(read_AT24C04(address + 1));
            puts("\n\r");
        }
    }
    CLEAR_DISP_SCREEN;
    clcd_print("DOWNLOAD COMPLETE",LINE1(0));
    __delay_ms(1000);
    CLEAR_DISP_SCREEN;
    menu_index = 0;
    flag1 = 1; 
}

void set_time()
{
    static unsigned int field = 0, once = 0;
    static unsigned int sec = 0, min = 0, hour = 0;
    if(once == 0)
    {
        get_time();
        once++;
        sec = (time[6] - '0')*10;
        sec = sec + (time[7] - '0');
        min = (time[3] - '0')*10;
        min = min + (time[4] - '0');
        hour = (time[0] - '0')*10;
        hour = hour + (time[1] - '0');
        CLEAR_DISP_SCREEN;
    }
    
    if(switch_value == SWITCH2)
    {
        field++;
        if(field == 3)
            field = 0;
    }
    else if(switch_value == SWITCH1 && field == 0)
    {
        if(sec < 59)
            sec++;
        else
            sec = 0;
    }
    else if(switch_value == SWITCH1 && field == 1)
    {
        if(min < 59)
            min++;
        else
            min = 0;
    }
    else if(switch_value == SWITCH1 && field == 2)
    {
        if(hour < 12)
            hour++;
        else
            hour = 0;
    }
    else if(switch_value == SWITCH11)
    {
        //store it in RTC
        write_ds1307(SEC_ADDR, (((sec/10)<<4)|(sec%10)));
        write_ds1307(MIN_ADDR, (((min/10)<<4)|(min%10)));
        write_ds1307(HOUR_ADDR, (((hour/10)<<4)|(hour%10)));
        once = 0;
        field = 0;
        flag1 = 1;
        menu_index = 0;
    }
    else if(switch_value == SWITCH12)
    {
        once = 0;
        field = 0;
        menu_index = 0;
        flag1 = 1;
    }
    
    //CLEAR_DISP_SCREEN;
    clcd_print("SET TIME",LINE1(4));
    if(blink_flag == 0)
    {
        clcd_putch((hour/10)+'0',LINE2(4));
        clcd_putch((hour%10)+'0',LINE2(5));
        clcd_putch(':',LINE2(6));
        clcd_putch((min/10)+'0',LINE2(7));
        clcd_putch((min%10)+'0',LINE2(8));
        clcd_putch(':',LINE2(9));
        clcd_putch((sec/10)+'0',LINE2(10));
        clcd_putch((sec%10)+'0',LINE2(11));
    }
    else
    {
        if(field == 0)
        {
            clcd_print("  ",LINE2(10));
        }
        else if(field == 1)
        {
            clcd_print("  ",LINE2(7));
        }
        else if(field == 2)
        {
            clcd_print("  ",LINE2(4));
        }
    }
}