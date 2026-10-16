#include<xc.h>
#include "external_eeprom.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "uart.h"
#include "ds1307.h"
#define EVENT_SIZE  12
#define VIEW_LOG 0
#define DOWNLOAD_LOG 1
#define CLEAR_LOG 2
#define SET_TIME 3
#define HOUR 0
#define MIN 1
#define SEC 2
extern unsigned char time[];
extern unsigned char event_index;
extern unsigned char event_count;
char main_menu_display[4][15]={"VIEW LOG        ","DOWNLOAD  LOG  ","CLEAR  LOG     ","SET  TIME      "};
char arrow[]="->";
void fun_main_menu()
{
    int index=0;
    CLEAR_DISP_SCREEN;
    clcd_print(arrow,LINE1(0));
    clcd_print(main_menu_display[index],LINE1(3));
    clcd_print(main_menu_display[index+1],LINE2(3));
    while(1)
    {
    int key=read_switches(STATE_CHANGE);
    if(key==MK_SW11)
    {
       execute_event(index);
        break;
    }
    else if(key==MK_SW12)
    {  
        break;
    }
    else if(key==MK_SW1)
    {
        index++;
        if(index==4)
            index=0;
    }
    else if(key==MK_SW2)
    {
        index--;
        if(index==-1)
            index=3;
    }
    if(index==0){
        clcd_print(arrow,LINE1(0));
        clcd_print("  ",LINE2(0));}
    else{
        clcd_print("  ",LINE1(0));
        clcd_print(arrow,LINE2(0));}
    clcd_print(main_menu_display[(index==0)?index:index-1],LINE1(3));
    clcd_print(main_menu_display[index==0?index+1:index],LINE2(3));
   }
   CLEAR_DISP_SCREEN;
   clcd_print("TIME", LINE1(2));
   clcd_print("E",LINE1(11));
   clcd_print("SP",LINE1(14));
   return ;
}

char event_database[10][17];
void execute_event(int option)
{
    if(option==VIEW_LOG)
    {
        read_event();
        display_event_in_clcd();
    }
    else if(option==DOWNLOAD_LOG)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("DOWNLOADING",LINE1(3));
        __delay_ms(3000);
        read_event();
        display_event_in_teratem();
    }
    else if(option==CLEAR_LOG)
    {
        CLEAR_DISP_SCREEN;
        clcd_print("LOG'S CLEARED",LINE1(3));
        __delay_ms(3000);
        event_count=0;
        event_index=0;
    }
    else if(option==SET_TIME)
    {
        set_time();
    }
    return;
}
void read_event(void)
{
    int count=0;
    for(int i=event_index;i<event_count;i++)
    {
        unsigned char address = i * EVENT_SIZE;
        unsigned char k=0;
        if(i==10)
        {
            break;
        }
        for(int j=0;j<16;j++)
        {
            if(j==0)
            {
                event_database[count][j]=count+'0';
            }
            else if(j==1||j==10||j==13)
            {
                event_database[count][j]=' ';
            }
            else
            {
            event_database[count][j]=read_external_eeprom(address+k++);}
        }
        event_database[count][16]='\0';
        count++;
    }
    for(int i=0;i<event_index;i++)
    {
        unsigned char address = i * EVENT_SIZE;
        unsigned char k=0;
        for(int j=0;j<16;j++)
        {
            if(j==0)
            {
                event_database[count][j]=count+'0';
            }
            else if(j==1||j==10||j==13)
            {
                event_database[count][j]=' ';
            }
            else
            {
            event_database[count][j]=read_external_eeprom(address+ k++);}
        }
        event_database[count][16]='\0';
        count++;
    }
}

void display_event_in_clcd()
{
    CLEAR_DISP_SCREEN;
     if(event_count == 0)
    {
        clcd_print("NO LOGS FOUND", LINE1(1));
        __delay_ms(1000);
        return;
    }
    int index=0;
    char arr1='>';
    if(event_count==1)
    {
        clcd_print(event_database[index],LINE1(0));
        clcd_putch(arr1,LINE1(1));
    }
    else{
    clcd_print(event_database[index],LINE1(0));
    clcd_print(event_database[index+1],LINE2(0));
    clcd_putch(arr1,LINE1(1));}
    while(1)
    {
    int key=read_switches(STATE_CHANGE);
  
    if(key==MK_SW12)
    {  
        break;
    }
    else if(key==MK_SW1)
    {
        index++;
        if(index==event_count)
            index=0;
    }
    else if(key==MK_SW2)
    {
        if(index>0)
        index--;
    }
    clcd_print(event_database[(index==0)?index:index-1],LINE1(0));
    clcd_print(event_database[index==0?index+1:index],LINE2(0));
    if(index==0){
        clcd_putch(arr1,LINE1(1));
        clcd_putch(' ',LINE2(1));}
    else{
        clcd_putch(' ',LINE1(1));
        clcd_putch(arr1,LINE2(1));}
    }
    return;
}

void display_event_in_teratem(void)
{
    for(int i=0;i<event_count;i++)
    {
        puts(event_database[i]);
        putch('\n');
    }
    return;
}

void set_time()
{
    CLEAR_DISP_SCREEN;
    char str[9]="HH:MM:SS";
    int hour=(time[0]-'0')*10 + (time[1]-'0');
    int min=(time[3]-'0')*10 + (time[4]-'0');
    int sec =(time[6]-'0')*10 + (time[7]-'0');
    clcd_print(str,LINE1(3));
    clcd_print(time,LINE2(3));
    int key=0;
    int delay=0;
    unsigned char field=HOUR;
    while(1)
    {
        key=read_switches(STATE_CHANGE);
        if(key==MK_SW1)
        {
            if(field==HOUR){
                if(hour==23)
                    hour=0;
                else
                    hour++;}
            else if(field==MIN)
            {
                if(min==59)
                {
                    min=0;
                    hour++;
                    if(hour==24)
                    {
                        hour=0;
                    }
                }
                else{
                min++;}
            }
            else if(field==SEC)
            {
                if(sec==59)
                {
                    sec=0;
                    min++;
                    if(min==60)
                    {
                        min=0;
                        hour++;
                        if(hour==24)
                        {
                            hour=0;
                        }
                    }
                }
                else
                sec++;    
            }  
        }
        if(key==MK_SW2)
        {
            if(field==HOUR){
                if(hour==0)
                    hour=23;
                else
                    hour--;}
            else if(field==MIN)
            {
                if(min==0)
                {
                    min=59;
                    hour--;
                    if(hour==0)
                    {
                        hour=23;
                    }
                }
                else{
                min--;}
            }
            else if(field==SEC)
            {
                if(sec == 0)
                {
                    sec = 59;
                    min--;
                    if(min == 0)
                    {
                        min = 59;
                        hour--;
                        if(hour == 0)
                        {
                            hour = 23;
                        }
                    }
                }
                else
                sec--;    
            }    
        }
        if(key==MK_SW3)
        {
            field++;
            if(field==3)
            {
                field=HOUR;
            }
        }
        if(key==MK_SW11)
        {
            
            save_time(hour,min,sec);
            break;
        }
        if(key==MK_SW12)
        {
            break;
        }
        delay++;
        clcd_putch(hour/10+'0',LINE2(3));
        clcd_putch(hour%10+'0',LINE2(4));
        clcd_putch(':',LINE2(5));
        clcd_putch(min/10+'0',LINE2(6));
        clcd_putch(min%10+'0',LINE2(7));
        clcd_putch(':',LINE2(8));
        clcd_putch(sec/10+'0',LINE2(9));
        clcd_putch(sec%10+'0',LINE2(10));
        if(delay<4000)
        {
            if(field==HOUR)
            {
                clcd_putch(0,LINE2(3));
                clcd_putch(0,LINE2(4));
            }
            else if(field==MIN)
            {
                 clcd_putch(0,LINE2(5));
                 clcd_putch(0,LINE2(6));
            }
            else if(field ==SEC)
            {
                clcd_putch(0,LINE2(8));
                clcd_putch(0,LINE2(9));
            }
        }
        else if(delay >8000)
        {
            delay=0;
        }
        
    }
    return;
}

void save_time(int hour,int min,int sec)
{
    unsigned char data=0;
    data=(min/10)<<4 | min%10;
    write_ds1307(MIN_ADDR,data);
    data=(sec/10)<<4 | sec%10;
    write_ds1307(SEC_ADDR,data);
    data=(hour/10)<<4 | hour%10;
    write_ds1307(HOUR_ADDR,data);
    return;
}