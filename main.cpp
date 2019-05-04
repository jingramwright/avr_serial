/*-----------------------------------------------------------------------------
                            _______ _    _  ______      
                            |_____|  \  /  |_____/      
                            |     |   \/   |    \_   
                            
                   _______ _______  ______ _____ _______
                   |______ |______ |_____/   |   |_____| |
                   ______| |______ |    \_ __|__ |     | |_____
					   
--------------------------------------------------------------------------------
OVERVIEW
--------------------------------------------------------------------------------
  This implements a serial UART, written for use on the Atmega328P. It will
  echo the bytes received via a serial terminal.
--------------------------------------------------------------------------------
VERSION HISTORY
--------------------------------------------------------------------------------
  VERSION         DATE         AUTHOR                DESCRIPTION
  -------         ----         ------                -----------
  1.0             04/05/19     J. Ingram-Wright      Initial version
------------------------------------------------------------------------------*/


#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 9600
#define BAUD_PRESCALE ((((F_CPU / 16) + (BAUDRATE / 2)) / (BAUDRATE)) - 1)


void initialiseSerial(void)
{
    UBRR0H = (uint8_t)(BAUD_PRESCALE >> 8); // Load upper 8-bits into UBRR register.
    UBRR0L = (uint8_t) (BAUD_PRESCALE); // Load lower 8-bits into UBRR register.
    UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable transmission and reception.
    UCSR0C = (3<<UCSZ00); // 8-bit character size, 1 stop bit, no parity.
}

uint8_t receiveSerial()
{
    while(!(UCSR0A & (1 << RXC0))); // Wait until data has been received and is available to be read.
    return UDR0;
}

void sendSerial(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0))); // Wait until UDR is available for data to be written.
    UDR0 = data;
}


int main(void)
{
    uint8_t data; // Used to hold receive and send data.

	initialiseSerial();

	while(1)
    {
	    data = receiveSerial();
        sendSerial(data);
    }   
	
	return 0;
}