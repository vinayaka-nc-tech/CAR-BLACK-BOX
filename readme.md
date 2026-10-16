Car Black Box

A PIC18F4580-based embedded project that records important driving
events with their time and speed, then lets the user review or download
the stored logs.

I built and tested this project on hardware using MPLAB X IDE and the
XC8 compiler. During testing, I debugged the issues I found and verified
the working features on the actual setup.

Overview

The Car Black Box keeps a small history of events in external EEPROM.
Each record includes a timestamp, an event identifier, and the speed
value captured by the system. A character LCD provides the dashboard and
menu interface, while the RTC maintains the time used for event records.

The project also includes a UART-based option to transfer stored logs to
a computer over a USB connection.

Features

Dashboard display --- displays the current speed, gear, and
time.

Speed input --- reads the speed input through the PIC18F4580's
ADC.

Gear selection --- allows the user to change gears using the
matrix keypad.

Collision event logging --- records a collision event for later
review.

Timestamped records --- uses the DS1307 RTC to associate events
with the time they occurred.

External EEPROM storage --- stores event records so they remain
available after the menu changes.

View Log --- browses saved event records on the character LCD.

Download Log --- sends stored records to a computer through UART
and the USB connection.

Clear Log --- clears the stored event history.

Set Time --- lets the user set the RTC time from the menu.

Limited event history --- maintains up to 10 event records,
using circular-buffer logic to reuse storage when the log is full.

Hardware Used

Component                           Purpose

PIC18F4580 microcontroller          Main controller

Character LCD (CLCD)                Dashboard, menu, and log display

DS1307 RTC                          Timekeeping for event timestamps

External EEPROM (24C02)             Persistent event-log storage

Matrix keypad / switches            Menu navigation, gear selection,
and event input

Potentiometer                       Provides an adjustable analog input
for speed

UART-to-USB connection              Transfers event logs to a computer

Software and Tools

Embedded C

MPLAB X IDE

XC8 compiler

I²C communication for the RTC and EEPROM

ADC for analog speed input

UART for log transfer

Menu Options

The LCD menu provides four operations:

VIEW LOG --- displays saved event records.

DOWNLOAD LOG --- transfers the stored records to a connected
computer.

CLEAR LOG --- clears the event history.

SET TIME --- adjusts the DS1307 RTC time.

The user can return to the dashboard after using the menu.

How It Works

The PIC18F4580 reads the analog speed input and updates the
dashboard.

The DS1307 RTC provides the current time.

When a supported event occurs, the controller stores its timestamp,
event identifier, and speed in external EEPROM.

The user can view saved records on the LCD or download them through
UART.

When the event history reaches its capacity, the circular-buffer
logic reuses the log storage for subsequent records.

Repository Structure

CAR_BLACK_BOX.X/
├── adc.c / adc.h
├── clcd.c / clcd.h
├── ds1307.c / ds1307.h
├── external_eeprom.c / external_eeprom.h
├── i2c.c / i2c.h
├── i2c1.h
├── main.c
├── main_menu.c
├── matrix_keypad.c / matrix_keypad.h
├── uart.c / uart.h
├── Makefile
└── nbproject/

The file list above reflects the main source modules. The repository may
contain additional project configuration files.

Open and Build the Project

Clone or download this repository.

Open MPLAB X IDE.

Select File → Open Project.

Browse to the CAR_BLACK_BOX.X project folder and open it.

Make sure the PIC18F4580 device and XC8 toolchain are configured in
the project.

Build the project in MPLAB X and program the microcontroller using
your compatible programmer/debugger.

The project was developed for MPLAB X IDE with the XC8 compiler. Device
configuration, compiler version, programmer, and hardware connections
may need to match your setup.

Testing

The project was tested on the physical hardware. Issues found during
testing were debugged, and the implemented features were checked on the
working setup.

Possible Improvements

Add photos of the hardware and LCD screens.

Include a wiring diagram and pin-connection table.

Add a short demonstration video showing event recording and log
download.

Author

Vinayaka N. C.

GitHub: vinayaka-nc-tech

This is an educational embedded-systems project demonstrating
microcontroller programming, peripheral interfacing, RTC-based
timestamps, EEPROM data storage, keypad-driven menus, LCD interfacing,
and UART communication.