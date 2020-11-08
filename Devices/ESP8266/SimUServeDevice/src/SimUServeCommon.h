#ifndef SimUServeCommon_h
#define SimUServeCommon_h

#include <Arduino.h>

#define ASCII_START_CHAR 32

// Define the IO ports
// + - may have unexpected behaviour at boot
// PORT DEFINE --     |   GPIO  |          Input           |           Output          |   Notes
#define ESP_D0 16 //  | GPIO16  | no interrupt             | no PWM or I2C support     | High at boot.  Used to wake from sleep mode.
#define ESP_D1 5  //  | GPIO5   | OK                       | OK                        | often used as SCL (I2C)
#define ESP_D2 4  //  | GPIO4   | OK                       | OK                        | often used as SDA (I2C)
#define ESP_D3 0  //  | GPIO0   | pulled up                | OK                        | connected to FLASH button, boot fails if pulled LOW
#define ESP_D4 2  //  | GPIO2   | pulled up                | OK                        | High at boot. connected to on-board LED, boot fails if pulled LOW
#define ESP_D5 14 //  | GPIO14  | OK                       | OK                        | SPI (SCLK)
#define ESP_D6 12 //  | GPIO12  | OK                       | OK                        | SPI (MISO)
#define ESP_D7 13 //  | GPIO13  | OK                       | OK                        | SPI (MOSI)
#define ESP_D8 15 //  | GPIO15  | Pulled to GND            | OK+                       | SPI (CS) - Boot fails if pulled HIGH
#define ESP_RX 3  //  | GPIO3   | OK+                      | RX pin                    | High at boot
#define ESP_TX 1  //  | GPIO1   | TX pin                   | OK+                       | High at boot - debug output at boot.  Boot fails if pulled LOW
#define ESP_A0 17 //  | ADC0    | Analog Input             | X                         | 

#endif