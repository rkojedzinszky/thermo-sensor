#include <avr/io.h>
#include "ev/interrupt.hpp"
#include "port.hpp"

typedef Pin<B, 0> led;

static void led_on();
static void led_off();

static void led_on()
{
	led::low();
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
		led::high();
		counter = 0;
	}
}

int main()
{
	PRR = ~_BV(PRTIM0);
	ACSR = _BV(ACD);
	DIDR0 = _BV(AIN1D) | _BV(AIN0D);

	led::mode(OUTPUT);
	led::high();

	WDTCR = _BV(WDP3);
	TCCR0B = _BV(CS01);

	sei();

	WDTInterrupt::enqueue(led_on);
	WDTInterrupt::loop();
}
