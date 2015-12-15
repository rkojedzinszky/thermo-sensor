#include <avr/io.h>
#include "ev/interrupt.hpp"

static void led_on();
static void led_off();

static void led_on()
{
	PORTB &= ~_BV(0);
	TIM0Interrupt::enqueue(led_off);
	TIM0Interrupt::loop();
	WDTInterrupt::enqueue(led_on);
}

static char counter = 0;
static void led_off()
{
	if (++counter < 3) {
		TIM0Interrupt::enqueue(led_off);
	} else {
		PORTB |= _BV(0);
		counter = 0;
	}
}

int main()
{
	PRR = ~_BV(PRTIM0);
	ACSR = _BV(ACD);
	DIDR0 = _BV(AIN1D) | _BV(AIN0D);

	DDRB |= _BV(PB0);
	PORTB = 0xff;

	WDTCR = _BV(WDP3);
	TCCR0B = _BV(CS01);

	sei();

	WDTInterrupt::enqueue(led_on);
	WDTInterrupt::loop();
}
