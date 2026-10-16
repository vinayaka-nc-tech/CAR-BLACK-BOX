#include <xc.h>
#include "i2c1.h"
#include "external_eeprom.h"
#define _XTAL_FREQ 20000000UL
void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();

    __delay_ms(5);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();

    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);

    i2c_rep_start();

    i2c_write(EEPROM_SLAVE_READ);
    data = i2c_read();

    i2c_stop();

    return data;
}