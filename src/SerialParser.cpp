#include <SerialParser.hpp>

SerialParser::SerialParser(EthernetClient& c, HardwareSerial& s, IPAddress i, IPAddress d, GPIO g, uint8_t de)
{
    this->__client  = &c;
    this->__serial  = &s;
    this->ip        = i;
    this->dns       = d;
    this->gpio      = g;
    this->is_debug  = de;
}

SerialParser::~SerialParser()
{
}

void SerialParser::debug(const char *fmt, ...) 
{
    if (is_debug) {
        va_list ap;
        char buffer[64];

        va_start(ap, fmt);
        vsprintf(buffer, fmt, ap);
        va_end(ap);

        __serial->print(buffer);
    }
}

void SerialParser::init( void )
{
    __serial->begin(115200);

    for (int i = 0; i < 16; i++)
    {
        pinMode(this->gpio.pinout[i], OUTPUT);
    }

    this->debug("Initialize Ethernet with DHCP.\n");
    
    Ethernet.begin(this->mac, ip, dns);

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        this->debug("Ethernet shield was not found.  Sorry, can't run without hardware. :(\n");
        while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        this->debug("Ethernet cable is not connected.\n");
    }

    debug("init done.\n");
}

void SerialParser::handle( void )
{
    byte c;
    uint8_t command_counter = 0;

    if (__serial->available() > 0) {
        do {
            c = __serial->read();
            if ((c != '\n') && (command_counter <= 4)) {
                this->command[command_counter] = c;
                command_counter++;
                delay(1);
            }
        } while (c != '\n');

        command_counter = 0;

        if (!checksum(this->command)) {
            return;
        }

        execute_command();

        __serial->println();
  }
}

bool SerialParser::checksum(byte *__command)
{
  unsigned int sum = __command[0] + __command[1] + __command[2];
  byte check = sum / 256 + sum % 256;

  debug("checksum : 0x%.2X, ", check);

  if (check == __command[3]) {
    debug(", OK !\n");
    return true;
  }

  debug(", NOK !\n");
  return false;
}

void SerialParser::execute_command()
{
    switch (this->command[0])
    {
        case 0x00:
            digitalWrite(this->gpio.pinout[this->command[1]], LOW);
            Serial.write("ACK");
            break;

        case 0x01:
            digitalWrite(this->gpio.pinout[this->command[1]], HIGH);
            Serial.write("ACK");
            break;
            
        default:
            Serial.write("NACK");
        break;
    }
}