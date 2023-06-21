#include <KernelParser.hpp>

KernelParser::KernelParser(EthernetServer& c, HardwareSerial& s, HardwareSerial& d_s, IPAddress i, IPAddress d, GPIO g, uint8_t de)
{
    this->__server  = &c;
    this->__serial  = &s;
    this->debug_serial = &d_s;
    this->ip        = i;
    this->dns       = d;
    this->gpio      = g;
    this->is_debug  = de;
}

void KernelParser::debug(const char *fmt, ...) 
{
    if (is_debug) {
        va_list ap;
        char buffer[64];

        va_start(ap, fmt);
        vsprintf(buffer, fmt, ap);
        va_end(ap);

        this->debug_serial->print(buffer);
    }
}

void KernelParser::init_hardware( void )
{
    __serial->begin(115200);
    debug_serial->begin(115200);

    for (int i = 0; i < 16; i++)
    {
        pinMode(this->gpio.pinout[i], OUTPUT);
        digitalWrite(this->gpio.pinout[i], HIGH);
    }

    this->debug("Initialize Ethernet with DHCP.\n");
    
    Ethernet.begin(this->mac, ip, dns);

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        this->debug("Ethernet shield was not found.  Sorry, can't run without hardware. :(\n");
        ethernet_hardware = false;
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        this->debug("Ethernet cable is not connected.\n");
        ethernet_hardware_cable = false;
    }

    debug("init done.\n");
}

byte KernelParser::hash_checksum(byte _cmd, byte _data_1, byte _data_2)
{
    unsigned int sum = _cmd + _data_1 + _data_2;
    byte check = sum / 256 + sum % 256;

    debug("hash_checksum -> 0x%.2X\n ", check);

    return check;
}

bool KernelParser::checksum(byte *__command)
{
  unsigned int sum = __command[0] + __command[1] + __command[2];
  byte check = sum / 256 + sum % 256;

  debug("checksum -> 0x%.2X, ", check);

  if (check == __command[3]) {
    debug(", OK !\n");
    return true;
  }

  debug(", NOK !\n");
  return false;
}

uint8_t KernelParser::hw_toggle_pin_write(uint8_t pin)
{
    if (pin < 0 || pin > 15)
    {
        debug("hw_write_pin -> pin %d out of range ... should be [0 15].\n\r", pin);
        return 0;
    }
    
    for (int i = 0; i < 16; i++)
    {
        if (i == pin) {
            digitalWrite(this->gpio.pinout[pin], LOW);
        } else {
            digitalWrite(this->gpio.pinout[i], HIGH);
        }
    }

    return 1;
    
}

uint8_t KernelParser::hw_pin_write(uint8_t pin, uint8_t state)
{
    if (pin < 0 || pin > 15) {
        debug("hw_write_pin -> pin %d out of range ... should be [0 15]. state : %.2X\n\r", pin, state);
        return 0;
    }
    
    switch (state)
    {
    case HIGH:
        digitalWrite(this->gpio.pinout[pin], LOW);
        break;
    
    case LOW:
        digitalWrite(this->gpio.pinout[pin], HIGH);
        break;

    default:
        debug("hw_write_pin -> pin %d state out of range : %.2X\n", pin, state);
        return 0;
        break;
    }

    return 1;
}

uint16_t KernelParser::read_outputs( void )
{
    debug("reads_outputs -> ");

    for (int i = 0; i < 16; i++)
    {
        this->gpio.state[i] = !digitalRead(this->gpio.pinout[i]);
        debug("[state : %d, pin : %d]\n", this->gpio.state[i], this->gpio.pinout[i]);
    }

    uint16_t result = 0;

    for (int i = 0; i < 16; i++)
    {
        result |= this->gpio.state[i] << i;
    }

    debug("read_outputs -> output states : %0.2X\n", result);
     
    return result;
}

void KernelParser::publish(uint8_t interface, uint8_t data)
{
    switch (interface)
    {
    case PUBLISH_OVER_ETHERNET:
        if (this->client)
        {
            this->client.write(data);
        }
        break;

    case PUBLISH_OVER_SERIAL:
        __serial->write(data);
        break;
    
    default:
        break;
    }
}

uint8_t KernelParser::send_packet(uint8_t interface, uint8_t cmd, uint16_t data)
{
    byte h, l;
    h = (data & 0xFF);
    l = (data >> 8);

    publish(interface, cmd);
    publish(interface, h);
    publish(interface, l);
    publish(interface, hash_checksum(cmd, h, l));
    publish(interface, 0xFF);

    return 1;
}