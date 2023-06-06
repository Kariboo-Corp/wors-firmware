#include <SerialParser.hpp>

#include <assets.hpp>

SerialParser parser(client, Serial, ip, myDns, pins, 1);

void setup() {
  parser.init();
}

byte c;
byte command[4] = { 0 };
unsigned int command_counter = 0;

void loop() {
  parser.handle();
}