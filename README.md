# WORS Firmware
---
This firmware is used as a development prototype.

## Technical Specification - GPIO Activation/Deactivation Protocol via Serial Port

### Introduction
This technical specification defines the communication protocol for activating or deactivating GPIO ports on an electronic board through a USB serial port. The protocol facilitates communication between the host computer and the electronic board, enabling control over the available 16 GPIO ports.

### Serial Communication
 - Interface

Serial communication utilizes the USB interface to connect the electronic board to the host computer.
Ensure that the board is recognized as a virtual serial port when connected to the computer.

 - 2.2 Communication Speed

The communication speed (baud rate) must be defined to establish the data transfer rate.
It is recommended to use a standard value such as 115200 baud.
Message Structure

 - 3.1 Message Format

GPIO activation/deactivation commands are sent as structured messages.
Each message consists of multiple bytes forming a predefined format.

 - 3.2 Message Structure

A message is structured as follows:
 - Byte 1: Operation code (e.g., 0x01 for activation, 0x02 for deactivation)
 - Byte 2: GPIO number (values from 0 to 15 for the 16 GPIO ports)
 - Byte 3: Additional data (if necessary)
 - Byte 4: Checksum for message integrity verification (optional)

### Supported Commands
 - 4.1 Operation Codes

Operation codes specify the action to be performed on the GPIO port.
The following operation codes are supported:
 - 0x01: Activate the specified GPIO port
 - 0x02: Deactivate the specified GPIO port
 - 0x03: Ask temperatures. Reply 2 x 4 Bytes : [0x03, temp(low), temp(high), checksum], temp = (temp(low) + temp(high) * 0xFF) / 100
 - 0x04: Ask outputs hardware state . Reply 1 x 4 Bytes : [0x04, state(16-8), state(7-0), checksum].

### Communication Protocol
 - 5.1 Message Exchange

The computer sends messages in the defined format to the electronic board to activate or deactivate GPIO ports.
The electronic board receives the messages, analyzes the operation code and GPIO number, and performs the appropriate action.

 - 5.2 Confirmation and Error Handling

The electronic board can send a confirmation message to the computer to indicate that the action has been successfully executed.
In case of errors, the electronic board can send an appropriate error message if an invalid command is received or if an error occurs during GPIO activation/deactivation.
Additional Recommendations

It is recommended to adhere to the relevant serial and USB protocol standards and recommendations to ensure compatibility and safety.
The technical documentation of the electronic board should include this specification to facilitate integration and use of the board.

## Technical Specification - GPIO Activation/Deactivation Protocol via TCP/IP

### Introduction
This technical specification defines the communication protocol for activating or deactivating GPIO ports on an electronic board through a TCP/IP connection. The protocol enables control over the available 16 GPIO ports by facilitating communication between the client application and the electronic board.

### TCP/IP Communication
 - 2.1 Connection

The communication is established using the TCP/IP protocol.
The electronic board acts as a server, while the client application initiates the connection.
 - 2.2 Network Configuration

The electronic board should be assigned a static IP address or use DHCP to obtain an IP address automatically.
The client application needs to know the IP address and port number to establish the connection.
Message Structure
 - 3.1 Format

GPIO activation/deactivation commands are sent as structured messages over the TCP/IP connection.
Each message consists of multiple bytes forming a predefined format.

 - 3.2 Message Structure

A message follows the following structure:
 - Byte 1: Operation code (e.g., 0x01 for activation, 0x02 for deactivation)
 - Byte 2: GPIO number (values from 0 to 15 for the 16 GPIO ports)
 - Byte 3: Additional data (if necessary)
 - Byte 4: Checksum for message integrity verification (optional)

### Supported Commands
 - 4.1 Operation Codes

Operation codes specify the action to be performed on the GPIO port.
The following operation codes are supported:
 - 0x01: Activate the specified GPIO port
 - 0x02: Deactivate the specified GPIO port
 - 0x03: Ask temperatures. Reply 2 x 4 Bytes : [0x03, temp(low), temp(high), checksum], temp = (temp(low) + temp(high) * 0xFF) / 100
 - 0x04: Ask outputs hardware state . Reply 1 x 4 Bytes : [0x04, state(16-8), state(7-0), checksum].

### Communication Protocol
 - 5.1 Connection Establishment

The client application establishes a TCP/IP connection with the electronic board using the board's IP address and port number.
 - 5.2 Message Exchange

The client application sends messages in the defined format to the electronic board to activate or deactivate GPIO ports.
The electronic board receives the messages, analyzes the operation code and GPIO number, and performs the appropriate action.
 - 5.3 Confirmation and Error Handling

The electronic board can send a response message to the client application to confirm the successful execution of the requested action.
In case of errors, the electronic board can send an appropriate error message if an invalid command is received or if an error occurs during GPIO activation/deactivation.
Additional Recommendations

Implement appropriate security measures, such as authentication and encryption, to ensure the integrity and confidentiality of the communication.
Document the network configuration and connection details in the technical documentation of the electronic board to guide users in establishing the connection.

---
## Build
To build this firmware, you will need PlatformIOwith VS Code installed. 

## Hardware
The prototype of the electronic board comprises an Arduino Mega microcontroller board integrated with an EthernetShield (Wiznet W5100). The EthernetShield enables the board to establish a TCP/IP connection. Additionally, the UART3 port of the Arduino Mega is connected to a USB to UART converter. This configuration emulates the functionality of a real USB port located on the board, allowing communication through the serial interface. Similarly, the Ethernet port on the Arduino Mega is utilized for network connectivity. It's worth noting that the primary USB port of the Arduino Mega is utilized as a debug probe, facilitating debugging and monitoring activities during the development process. This setup enables the prototype board to communicate over TCP/IP and it USB port in order to establish a connection with external devices or applications.