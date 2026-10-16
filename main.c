#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include"adc.h"
#include"matrix_keypad.h"
#include "i2c1.h"
#include "external_eeprom.h"
#include "uart.h"
#define MAX_EVENTS  10
#define EVENT_SIZE  12
#define DASHBOARD 0
#define MAIN_MENU 1
unsigned char clock_reg[3];
unsigned char calender_reg[4];
    unsigned char time[9];
unsigned char date[11];
char gear_data[9][3]={"ON","GN","G1","G2","G3","G4","G5","GR","++"};
unsigned char event_index = 0;
unsigned char event_count = 0;
void display_time(void)
{
    clcd_print(time, LINE2(2));
}

static void get_time(void)
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

void init_config(void)
{
    init_clcd();
    init_i2c();
    init_ds1307();
    init_adc();
    init_matrix_keypad();
    init_uart();
    clcd_print("TIME", LINE1(2));
    clcd_print("EV",LINE1(11));
    clcd_print("SP",LINE1(14));
}

void store_event(unsigned char *time,
                 unsigned char *gear,
                 unsigned char speed)
{
    unsigned char address;
    unsigned char i;
    unsigned char speed_tens;
    unsigned char speed_units;

    address = event_index * EVENT_SIZE;

    for (i = 0; i < 8; i++)
    {
        write_external_eeprom(address + i, time[i]);
    }
    write_external_eeprom(address + 8, gear[0]);
    write_external_eeprom(address + 9, gear[1]);

    speed_tens = (speed / 10) % 10;
    speed_units = speed % 10;

    write_external_eeprom(address + 10, '0' + speed_tens);
    write_external_eeprom(address + 11, '0' + speed_units);

    event_index++;

    if (event_index >= MAX_EVENTS)
    {
        event_index = 0;
    }
    if (event_count < MAX_EVENTS)
    {
        event_count++;
    }
}


unsigned char field=0;
void main(void)
{
    unsigned int speed=0;
    unsigned int key=0;
    unsigned char gear=0;
    init_config();

    while (1)
    {
        get_time();
        display_time();
        speed = read_adc(4) / 10.34;
        clcd_putch('0'+speed/10,LINE2(14));
        clcd_putch('0'+speed%10,LINE2(15));
        key=read_switches(STATE_CHANGE);
        if(key==MK_SW11)
        {
            field = MAIN_MENU;
        }
        else if(key==MK_SW12)
        {
            field=DASHBOARD;
        }
        if(field==DASHBOARD)
        {
        if(key==MK_SW1)
        {
            if(gear==8) gear=0;
            else if(gear<7){gear++;
            store_event(time,gear_data[gear], speed);}
        }
        else if(key==MK_SW2)
        {
            if(gear==8)
                gear=0;
            else if (gear>0){
                gear--;
            store_event(time,gear_data[gear], speed);}
        }
        else if(key==MK_SW3)
        {
            gear=8;
            store_event(time,gear_data[gear], speed);
        }
        clcd_print(gear_data[gear],LINE2(11));
        }
        
        else if(field==MAIN_MENU)
        {
            fun_main_menu();
            field=DASHBOARD;
        }
    }
}