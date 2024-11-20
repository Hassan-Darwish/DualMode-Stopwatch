//Hassan Darwish Standard Diploma 91

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// functions prototypes
void TIMER1_INIT(void);
void INT0_INIT(void);
void INT1_INIT(void);
void INT2_INIT(void);
void Seven_Segment_Displays(void);
void Increment_Decrement_PUSH_BUTTONS(void);


//global variables
unsigned char seconds = 0x00;
unsigned char minutes = 0x00;
unsigned char hours = 0x00;
unsigned char state_flag = 0x00;
unsigned char buzzer_flag = 0x00;// max or min to enable buzzer
unsigned char pause_flag = 0x00;

int main(void)
{
	/*
	 PORTA are all outputs
	 -power source of the 7-seg
	 */
	DDRA |= (0x3F);
	PORTA &= ~(0x3F); // disables all 7-segs
	/*
	 PORTB are all inputs
	 -clock increment and decrement
	 -resume stopwatch
	 -count mode
	 */
	DDRB &= ~(0xFF);
	PORTB |= (0xFF);//internal pull-up enable
	/*
	 PORTC are all outputs
	 -PC0-PC3 74LS47 encoder
	 */
	DDRC |= 0x0F;
	PORTC |= 0x00;// init the 7 segs to 0's
	/*
	 PORTD consists of 3 outputs and 2 input
	 -PD0 buzzer output
	 -PD2 reset stopwatch input
	 -PD3 pause stopwatch input (pull down)
	 -PD4 count up LED output
	 -PD5 count down LED output
	 */
	DDRD |= 0x31;
	DDRD &= ~0x0C;
	PORTD |= (1<<PD2);//internal pull-up enable
	PORTD &= ~0x31;//disable LEDs and buzzer



	TIMER1_INIT();
	INT0_INIT();
	INT1_INIT();
	INT2_INIT();

	for(;;)
	{
		Seven_Segment_Displays();

		if(!(PINB & (1<<PB7)))//Count Mode pushbutton checker
		{
			_delay_ms(30);

			if(!(PINB & (1<<PB7)))
			{
				state_flag ^= 1;//toggle between modes
				buzzer_flag = 0;// deactivate the buzzer
				while(!(PINB & (1<<PB7)))
				{Seven_Segment_Displays();}
			}
		}


		if(state_flag == 0x00)//checks if its counting up
		{
			PORTD |= 0x10;//LED-RED Enabled
			PORTD &= ~0x20;//LED-YELLOW Disabled

			if(buzzer_flag == 1)//enables the buzzer
			{
				PORTD |= 0x01;
			}
			else
			{
				PORTD &= ~0x01;
			}
		}
		else
		{
			PORTD |= 0x20;//LED-YELLOW ENABLE
			PORTD &= ~(0x10);//LED-RED DISABLE

			if(buzzer_flag == 1)
			{
				PORTD |= 0x01;
			}
			else
			{
				PORTD &= ~0x01;
			}
		}
		Increment_Decrement_PUSH_BUTTONS();
	}
}
void TIMER1_INIT(void)
{
	TCCR1B |= (1<<WGM12);//CTC at OCR1A
	TCCR1B |= (1<<CS12) | (1<<CS10);// 1024 prescaler
	OCR1A = 15625;//limit of comparitor
	SREG |= (1<<7);
	TIMSK |= (1<<OCIE1A);
}
void INT0_INIT(void)
{
	MCUCR |= (1<<ISC11);//falling edge
	MCUCR &= ~(1<<ISC10);
	SREG |= (1<<7);// interrupt_bit enable
	GICR |= (1<<INT0);

}
void INT1_INIT(void)
{
	MCUCR |= (1<<ISC10);//rising edge
	MCUCR |= (1<<ISC11);
	SREG |= (1<<7);
	GICR |= (1<<INT1);
}
void INT2_INIT(void)
{
	MCUCSR &= ~(1<<ISC2);//falling edge
	SREG |= (1<<7);
	GICR |= (1<<INT2);
}
ISR(TIMER1_COMPA_vect)
{
	if(state_flag == 0x00 && pause_flag == 0x00) //stopwatch increment logic
	{
		if(seconds != 59)
		{
			seconds++;
		}
		else if(minutes != 59)
		{
			seconds = 0x00;
			minutes ++;
		}
		else if(hours != 99){
			seconds = 0x00;
			minutes = 0x00;
			hours ++;
		}
		else
		{
			buzzer_flag = 1;
		}
	}
	else if(state_flag != 0x00 && pause_flag == 0x00) //stopwatch decrement logic
	{
		if(seconds != 0)
		{
			seconds--;
		}
		else if(minutes != 0)
		{
			seconds = 59;
			minutes --;
		}
		else if(hours != 0){
			seconds = 59;
			minutes = 59;
			hours --;
		}
		else
		{
			buzzer_flag = 1;
		}
	}
	else if(state_flag != 0x00 && pause_flag != 0x00){}// pauses the alarm
}
ISR(INT0_vect)//reset Interrupt
{
	seconds = 0;
	minutes = 0;
	hours = 0;
}
ISR(INT1_vect)//pause interrupt
{
	pause_flag = 1;
}
ISR(INT2_vect)//resume interrupt
{
	pause_flag = 0;
}
void Seven_Segment_Displays(void)
{
	PORTA = 0x20;//display on first sec 7-seg
	PORTC = seconds%10;
	_delay_us(50);
	PORTA = 0x10;//display on second sec 7-seg
	PORTC = seconds/10;
	_delay_us(50);

	PORTA = 0x08;//display on first min 7-seg
	PORTC = minutes%10;
	_delay_us(50);
	PORTA = 0x04;//display on second min 7-seg
	PORTC = minutes/10;
	_delay_us(50);

	PORTA = 0x02;//display on first hr 7-seg
	PORTC = hours%10;
	_delay_us(50);
	PORTA = 0x01;//display on second min 7-seg
	PORTC = hours/10;
	_delay_us(50);
}
void Increment_Decrement_PUSH_BUTTONS(void)
{
	if(!(PINB & (1<<PB6)))//seconds increment
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB6)))
		{
			if(seconds != 59)
			{
				seconds ++;
			}
			while(!(PINB & (1 << PB6)))
			{Seven_Segment_Displays();}
		}
	}
	if(!(PINB & (1<<PB5)))//seconds decrement
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB5)))
		{
			if(seconds != 0x00)
			{
				seconds --;
			}
			while(!(PINB & (1 << PB5)))
			{Seven_Segment_Displays();}
		}
	}

	if(!(PINB & (1<<PB4)))//minutes increment
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB4)))
		{
			if(minutes != 59)
			{
				minutes ++;
			}
			while(!(PINB & (1 << PB4)))
			{Seven_Segment_Displays();}
		}
	}
	if(!(PINB & (1<<PB3)))//minutes decrement
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB3)))
		{
			if(minutes != 0x00)
			{
				minutes --;
			}
			while(!(PINB & (1 << PB3)))
			{Seven_Segment_Displays();}
		}
	}

	if(!(PINB & (1<<PB1)))//hours increment
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB1)))
		{
			if(hours != 99)
			{
				hours ++;
			}
			while(!(PINB & (1 << PB1)))
			{Seven_Segment_Displays();}
		}
	}
	if(!(PINB & (1<<PB0)))//hours decrement
	{
		_delay_ms(30);

		if(!(PINB & (1<<PB0)))
		{
			if(hours != 0x00)
			{
				hours --;
			}
			while(!(PINB & (1 << PB0)))
			{Seven_Segment_Displays();}
		}
	}
}
