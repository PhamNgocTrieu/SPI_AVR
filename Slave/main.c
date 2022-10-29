#define F_CPU 16000000UL 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <myLCD.h>

#ifndef  cbi
	#define cbi(sfr, bit)	sfr &= ~(1<<bit)
#endif

#ifndef  cbi
	#define sbi(sfr, bit)	sfr |= (1<<bit)
#endif

// PORTB is used for SPI interface
#define SLA 2

#define MOSI 3
#define MISO 4
#define SCK  5

volatile char wData = 247, rData;
char dis[5];

void SPI_Slave_init(){
	DDRB = (1<<MISO);
	PORTB |= (1<<SLA)|(1<<MOSI);
	SPCR = (1<<SPIE)|(1<<SPE)|(1<<CPHA);
}


int main(void)
{
	sei();
	SPI_Slave_init();
	init_LCD();
	clr_LCD();
	move_LCD(1,1); print_LCD("readDATA: ");
	move_LCD(2,1); print_LCD("writeDATA: ");
	sprintf(dis, "%i", wData);
	print_LCD(dis);	
    /* Replace with your application code */
	SPDR = wData;
    while (1) 
    {

    }
}

ISR(SPI_STC_vect){
	rData = SPDR;
	sprintf(dis, "%i", rData);
	move_LCD(1,11);
	print_LCD(dis);
}

