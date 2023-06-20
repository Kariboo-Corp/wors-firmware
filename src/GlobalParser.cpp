#include <GlobalParser.hpp>

void GlobalParser::init( void )
{
    init_hardware();
    __server->begin();
    debug("init_ethernet -> server started on port %d\n", this->server_port);
}

void GlobalParser::service_serial_handle( void )
{
    byte c;
    uint8_t command_counter = 0;

    if (__serial->available() > 0) {
        do {
            c = __serial->read();
            if ((c != 0xFF) && (command_counter <= 4)) {
                this->command[command_counter] = c;
                command_counter++;
                delay(1);
            }
        } while (c != 0xFF);

        command_counter = 0;

        if (!checksum(this->command)) {
            this->nack(PUBLISH_OVER_SERIAL);
            return;
        }

        execute_command(PUBLISH_OVER_SERIAL);

        __serial->println();
  }
}

void GlobalParser::service_ehtnernet_handle( void )
{
    EthernetClient client = __server->available();
    this->client = client;

    if (Ethernet.linkStatus() == LinkOFF) {
        this->debug("Ethernet cable is not connected.\n");
        ethernet_hardware_cable = false;
    }

    char c;
    int command_counter = 0;
    
    if (ethernet_hardware)
    {
        if (ethernet_hardware_cable)
        {
            if (client) {
                if (client.connected())  
                {
                    if (client.available() > 0) {
                        do {
                            c = client.read();
                            debug("service_handle -> byte reveiced : %0.2X\n", c);
                            if ((c != 0xFF) && (command_counter <= 4)) {
                                this->command[command_counter] = c;
                                command_counter++;
                                delay(1);
                            } else {
                                break;
                            }
                        } while (c != 0xFF);

                        command_counter = 0;

                        if (!checksum(this->command)) {
                            this->nack(PUBLISH_OVER_ETHERNET);
                            return;
                        }

                        execute_command(PUBLISH_OVER_ETHERNET);

                        client.println();
                    }
                }
            }
            this->client.stop();
        }
    }
}

void GlobalParser::execute_command(uint8_t interface)
{
    switch (this->command[0])
    {
        case CMD_SET_PIN_LOW: // disable output
            if (hw_pin_write(this->command[1], HIGH)) // SPDT HRF switches are in inverse logic
            {
                ack(interface);
            } else 
                nack(interface);
            break;

        case CMD_SET_PIN_HIGH: // enable output
            if (hw_pin_write(this->command[1], LOW)) // SPDT HRF switches are in inverse logic
            {
                ack(interface);
            } else 
                nack(interface);
            break;
        
        case CMD_TOGGLE_PIN: // toggle 'pin' HIGH, LOW otherwise
            if (this->hw_toggle_pin_write(command[1]))
            {
                ack(interface);
            } else 
                nack(interface);
            break;

        case CMD_GET_TEMPERATURE: // get temperature
            publish_temperatures(interface);
            ack(interface);
            break;

        case CMD_GET_OUTPUTS: // read outputs
            publish_outputs(interface);
            ack(interface);
            break;
        case CMD_BOARD_IDENTIFY:
            send_packet(interface, CMD_BOARD_IDENTIFY, BOARD_ID);
            ack(interface);
            break;
            
        default:
            debug("execute_command -> can't find any command for %.2X ...\n", command[0]);
            nack(interface);
            break;
    }
}

void GlobalParser::publish_temperatures(uint8_t interface)
{

    float simulated_temp_1, simulated_temp_2;
    uint16_t temp1, temp2;

    simulated_temp_1 = 32.9;
    simulated_temp_2 = 39.8;

    temp1 = simulated_temp_1 * 100; // should not overflow 16 bits (65,535)
    temp2 = simulated_temp_2 * 100;

    send_packet(interface, CMD_GET_TEMPERATURE, temp1);
    send_packet(interface, CMD_GET_TEMPERATURE, temp2);

}

void GlobalParser::publish_outputs(uint8_t interface)
{
    uint16_t data = read_outputs();
    send_packet(interface, CMD_GET_OUTPUTS, data);
}