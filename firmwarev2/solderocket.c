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

uint16_t delays[]=
{
	50,
	70,
	97,
	135,
	189,
	264,
	368,
	513,
	716,
	1000,
};

int main(void)
{
	InitADC();
	DDRC=0x04;
	PORTC=0x00;
	DDRD=0xFF;
	PORTD=0x00;
	DDRB=0xFF;
	PORTB=0x00;
	while(1)
	{
		uint8_t setting;
		setting = ReadADC(0)/93; // 11 values 0-10
		if (setting == 11)
			setting=10;

		uint8_t lastbutton = 0;
		uint8_t currbutton;
		currbutton = PINC&0x02;
		if (lastbutton != currbutton)
		{
			if (!currbutton)
				PORTC&=~(1<<2);
			else
			{
				PORTC|=(1<<2);
				if(setting < 10)
				{
					uint16_t delay=delays[setting]; //don't reread
					for(uint16_t count=delay;count;count--)
						_delay_ms(1); // needs compile time value
					PORTC&=~(1<<2);
				}
			};
			lastbutton = currbutton;
			_delay_ms(50);
		};
		
		for(uint8_t led=0;led<10;led++)
			if((setting == 10) ? 1 : (setting == led))
				*LEDS[led].port|=LEDS[led].mask;
			else
				*LEDS[led].port&=~LEDS[led].mask;
	};
}
