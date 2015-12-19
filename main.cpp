#include <avr/io.h>
#include "ev/interrupt.hpp"
#include "port.hpp"
#include "am2302.hpp"

typedef Pin<Port<B>, 1> led;
typedef Pin<Port<B>, 0> led2;
typedef AM2302< Pin<Port<B>, 4> > Tsensor1;

static void write_eeprom(int addr, unsigned char value)
{
	EECR = 0;

	EEAR = addr;
	EEDR = value;

	EECR |= _BV(EEMPE);
	EECR |= _BV(EEPE);

	loop_until_bit_is_clear(EECR, EEPE);
}

static void nop()
{
}

static short read_adc()
{
	ADCInterrupt::enqueue(nop);
	ADCInterrupt::loop();

	return ADCH << 8 | ADCL;
}

static void collect()
{
	static int addr = 0;
	short temp, hum;
	short voltage;

	led::clear();

	/*
	PRR &= ~_BV(PRADC);
	ADCSRA |= _BV(ADEN);
	ADMUX = _BV(REFS1) | _BV(MUX1) | _BV(MUX0);
	DIDR0 &= ~_BV(ADC3D);
	*/

	if (!Tsensor1::read(hum, temp)) {
		led2::clear();
		temp = hum = 0xfaaf;
	} else {
		led2::set();
	}

	if (addr >= 512) {
		addr = 0;
	}

	write_eeprom(addr++, (temp >> 8));
	write_eeprom(addr++, (temp & 0xff));
	write_eeprom(addr++, (hum >> 8));
	write_eeprom(addr++, (hum & 0xff));

	/*
	voltage = read_adc();
	if (voltage == 1023) {
		led2::clear();
	}

	DIDR0 |= _BV(ADC3D);
	ADCSRA &= ~_BV(ADEN);
	PRR |= _BV(PRADC);

	write_eeprom(addr++, (voltage >> 8));
	write_eeprom(addr++, (voltage & 0xff));
	*/

	write_eeprom(addr, 0xff);
	write_eeprom(addr+1, 0xff);

	led::set();
}

static void schedule()
{
	static unsigned char ticks = 0;

	if (++ticks == 1) {
		collect();
		ticks = 0;
	}

	WDTInterrupt::enqueue(schedule);
}

static void init()
{
	collect();
	WDTCR = _BV(WDP3) | _BV(WDP0); // 8 sec
	WDTInterrupt::enqueue(schedule);
}

int main()
{
	PRR = ~_BV(PRTIM0);
	ACSR = _BV(ACD);
	DIDR0 = _BV(ADC0D) | _BV(ADC2D) | _BV(ADC3D) | _BV(ADC1D) | _BV(AIN1D) | _BV(AIN0D);
	ADCSRA |= _BV(ADPS2);

	WDTCR = _BV(WDP2) | _BV(WDP1) | _BV(WDP0); // 2 secs
	// WDTCR = _BV(WDP3); // 4 secs
	TCCR0B = _BV(CS01);

	led::mode(OUTPUT);
	led::set();

	led2::mode(OUTPUT);
	led2::set();

	sei();

	WDTInterrupt::enqueue(init);
	WDTInterrupt::loop();
}
