#ifndef PINS_H
#define PINS_H

#include <Ethernet.h>

IPAddress ip(192, 168, 31, 16);
IPAddress myDns(192, 168, 31, 1);

EthernetClient client;

struct GPIO
{
    uint8_t pinout[16];
    bool    state[16];
};

struct GPIO gpio = {
     { 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53 },
     { 0 }
};

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#endif