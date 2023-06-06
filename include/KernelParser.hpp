#ifndef KERNEL_PARSER_H
#define KERNEL_PARSER_H

#include <Arduino.h>
#include <Ethernet.h>

#include <stdio.h>
#include <stdarg.h>

#define CMD_SET_PIN_LOW  0x00
#define CMD_SET_PIN_HIGH 0x01
#define CMD_TOGGLE_PIN   0x02
#define CMD_GET_TEMPERATURE 0x03
#define CMD_GET_OUTPUTS  0x04

#define PUBLISH_OVER_ETHERNET 0x00
#define PUBLISH_OVER_SERIAL   0x01

struct GPIO
{
    uint8_t pinout[16];
    bool    state[16];
};


class KernelParser
{
    private:
        IPAddress ip;
        IPAddress dns;

        byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
        uint8_t is_debug = 0;

    public:
        explicit KernelParser(EthernetServer&, HardwareSerial&, HardwareSerial&, IPAddress, IPAddress, GPIO, uint8_t);

        EthernetServer* __server;
        EthernetClient client;

        HardwareSerial* __serial;
        HardwareSerial* debug_serial;

        byte command[4] = { 0 };
        GPIO gpio;

        void debug(const char *fmt, ...);

        bool checksum(byte *);
        byte hash_checksum(byte, byte, byte);

        void init_hardware( void );

        void hw_toggle_pin_write(uint8_t);
        void hw_pin_write(uint8_t, uint8_t);
        uint16_t read_outputs( void );

        void publish(uint8_t, uint8_t);
};


#endif