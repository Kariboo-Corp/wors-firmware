#ifndef SERIAL_PARSER_H
#define SERIAL_PARSER_H

#include <Arduino.h>
#include <Ethernet.h>

#include <stdio.h>
#include <stdarg.h>

struct GPIO
{
    uint8_t pinout[16];
    bool    state[16];
};


class SerialParser
{
    private:
        EthernetClient* __client;
        HardwareSerial* __serial;

        IPAddress ip;
        IPAddress dns;

        byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

        GPIO gpio;

        byte command[4] = { 0 };
        uint8_t is_debug = 0;

        void execute_command();
        bool checksum(byte *);

        void debug(const char *fmt, ...);
        
    public:
        SerialParser(EthernetClient&, HardwareSerial&, IPAddress, IPAddress, GPIO, uint8_t);
        ~SerialParser();

        void init( void );
        void handle( void );
};


#endif