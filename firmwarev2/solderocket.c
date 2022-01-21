#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

static void InitADC(void)
{
 // Select Vref=AVcc
 ADMUX = (1<<REFS0);
 //set prescaller to 128 and enable ADC 
 ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);    
}

uint16_t ReadADC(uint8_t ADCchannel)
{
 //select ADC channel with safety mask
 ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
 //single conversion mode
 ADCSRA |= (1<<ADSC);
 // wait until ADC conversion is complete
 while( ADCSRA & (1<<ADSC) );
 return ADC;
}

struct {
	volatile uint8_t * port;
	uint8_t mask;
} LEDS[10] =
{
	{&PORTD,0x04},
	{&PORTD,0x08},
	{&PORTD,0x10},
	{&PORTD,0x20},
	{&PORTD,0x40},
	{&PORTD,0x80},
	{&PORTB,0x01},
	{&PORTB,0x02},
	{&PORTB,0x04},
	{&PORTB,0x08},
};

typedef struct {
	uint16_t delay;
	uint16_t led;
} Delays;

//[ round(50*math.exp(x)) for x in [math.log(1000/50)*x/18 for x in range(19)]]

Delays delays[]=
{
	{  50, 0b0000000001},
	{  59, 0b0000000011},
	{  70, 0b0000000010},
	{  82, 0b0000000110},
	{  97, 0b0000000100},
	{ 115, 0b0000001100},
	{ 136, 0b0000001000},
	{ 160, 0b0000011000},
	{ 189, 0b0000010000},
	{ 224, 0b0000110000},
	{ 264, 0b0000100000},
	{ 312, 0b0001100000},
	{ 368, 0b0001000000},
	{ 435, 0b0011000000},
	{ 514, 0b0010000000},
	{ 607, 0b0110000000},
	{ 717, 0b0100000000},
	{ 847, 0b1100000000},
	{1000, 0b1000000000},
};

#define DELAYS_LEN 19

int main(void)
{
	InitADC();
	DDRC=0x04;
	PORTC=0x00;
	DDRD=0xFF;
	PORTD=0x00;
	DDRB=0xFF;
	PORTB=0x00;
	DIDR0=0x01;
	uint8_t lastbutton = 0;	
	while(1)
	{
		uint8_t setting;
		setting = ReadADC(0)/((uint16_t)(1024.0/(DELAYS_LEN)));
		if (setting >= (DELAYS_LEN))
			setting=(DELAYS_LEN);

		uint8_t currbutton;
		currbutton = PINC&0x02;
		if (lastbutton != currbutton)
		{
			if (!currbutton)
				PORTC&=~(1<<2);
			else
			{
				PORTC|=(1<<2);
				if(setting < DELAYS_LEN)
				{
					uint16_t delay=delays[setting].delay; //don't reread
					for(uint16_t count=delay;count;count--)
						_delay_ms(1); // needs compile time value
					PORTC&=~(1<<2);
				}
			};
			lastbutton = currbutton;
			_delay_ms(50);
		};
		
		for(uint8_t led=0;led<10;led++)
			if((setting == (DELAYS_LEN)) ? 1 : (delays[setting].led & (1<<led)))
				*LEDS[led].port|=LEDS[led].mask;
			else
				*LEDS[led].port&=~LEDS[led].mask;
	};
}
