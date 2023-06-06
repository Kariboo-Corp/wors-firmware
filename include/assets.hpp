#ifndef PINS_H
#define PINS_H

IPAddress ip(192, 168, 31, 16);
IPAddress myDns(192, 168, 31, 1);

EthernetClient client;

struct GPIO pins = {
     { 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53 },
     { 0 }
};

void parse_command(byte*);
bool checksum(byte*);

#endif