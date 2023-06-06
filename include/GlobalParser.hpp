#ifndef GLOBAL_PARSER_H
#define GLOBAL_PARSER_H

#include <KernelParser.hpp>

class GlobalParser : private KernelParser
{
    using KernelParser::KernelParser;
    private:
        void ack(uint8_t interface)
        { 
            publish(interface, 'A');
            publish(interface, 'C');
            publish(interface, 'K');
        }

        void nack(uint8_t interface)
        { 
            publish(interface, 'N');
            publish(interface, 'A');
            publish(interface, 'C');
            publish(interface, 'K');
        }
    public:
        uint16_t server_port = 3033;
        
        void init( void );
        void service_ehtnernet_handle( void );
        void service_serial_handle( void );
        void execute_command(uint8_t interface);

        void publish_temperatures(uint8_t interface);
        void publish_outputs(uint8_t interface);
};


#endif