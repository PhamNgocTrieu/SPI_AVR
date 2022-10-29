
#define F_CPU 12000000UL
#include <avr/io.h>
#include <stdio.h>
#include <myLCD.h>
#include <util/delay.h>

#ifndef  cbi
	#define cbi(sfr, bit)	sfr &= ~(1<<bit)
#endif
#ifndef  cbi
	#define sbi(sfr, bit)	sfr |= (1<<bit)

#endif

// PORTB is used for SPI interface 
#define SLA(i)	i

#define MOSI 3
#define MISO 4
#define SCK  5

volatile uint8_t data[3] = {05, 10, 15};
	
// Start SPI at Master mode
void SPI_master_init(){
	DDRB = (1<<SCK)|(1<<MOSI)|(1<<SLA(2))|(1<<SLA(1))|(1<<SLA(0));;
	PORTB |= (1<<MISO) /* pull-up resistor*/ | (1<<SLA(2))|(1<<SLA(1))|(1<<SLA(0)); // Disable Slave Select pins
	SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
	SPSR |= (1<<SPI2X);// SCK frequency: F/64
}

uint8_t SPI_transmit(uint8_t i, uint8_t data){
	cbi(PORTB, SLA(i));
	SPDR = data;
	while(!(SPSR & (1<<SPIF))); // wait for reception complete
	sbi(PORTB, SLA(i));
	return SPDR;
}

int main(void)
{
	DDRC = 0x01;
	PORTC = 0x01;
	uint8_t temp = 0;
	char dis[5];
    SPI_master_init();
	_delay_ms(100);
	init_LCD();
	clr_LCD();
	move_LCD(1,1); print_LCD("writeDATA: ");
	sprintf(dis, "%i", data[0]);
	print_LCD(dis);
	move_LCD(2,1); print_LCD("readDATA:");
	uint8_t rd = SPI_transmit(0,data[0]);
	sprintf(dis, "%i", rd);
	print_LCD(dis);
	
    while (1) 
    {
		if (!(PINC & (1<<0)))
		{
			while(!(PINC & (1<<0)));
			temp++;
			if (temp > 2) temp = 0;
	
			if (temp == 0)
			{
				clr_LCD();
				move_LCD(1,1); print_LCD("writeDATA: ");
				sprintf(dis, "%i", data[0]);
				print_LCD(dis);
				move_LCD(2,1); print_LCD("readDATA:");
				uint8_t rd = SPI_transmit(0,data[0]);
				sprintf(dis, "%i", rd);
				print_LCD(dis);
			}
			
			if (temp == 1)
			{
				clr_LCD();
				move_LCD(1,1); print_LCD("writeDATA: ");
				sprintf(dis, "%i", data[1]);
				print_LCD(dis);
				move_LCD(2,1); print_LCD("readDATA:");
				uint8_t rd = SPI_transmit(1,data[1]);
				sprintf(dis, "%i", rd);
				print_LCD(dis);
			}
			
			if (temp == 2)
			{
				clr_LCD();
				move_LCD(1,1); print_LCD("writeDATA: ");
				sprintf(dis, "%i", data[2]);
				print_LCD(dis);
				move_LCD(2,1); print_LCD("readDATA:");
				uint8_t rd = SPI_transmit(2,data[2]);
				sprintf(dis, "%i", rd);
				print_LCD(dis);
			}
			
		}
    }
}

