#include <KernelParser.hpp>
#include <GlobalParser.hpp>

#include <assets.hpp>

GlobalParser sparser(server, Serial3, Serial, ip, myDns, pins, 1);

void setup() {
  sparser.init();
}

byte c;
byte command[4] = { 0 };
unsigned int command_counter = 0;

void loop() {
  sparser.service_serial_handle();
  sparser.service_ehtnernet_handle();
}