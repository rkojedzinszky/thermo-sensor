#include <avr/io.h>
#include "ev/interrupt.hpp"
#include "port.hpp"
#include "am2302.hpp"

typedef Pin<B, 1> led;
typedef Pin<B, 0> led2;
typedef AM2302< Pin<B, 2> > Tsensor1;

static void led_off()
{
	led::set();
}

static void write_eeprom(int addr, unsigned char value)
{
	EECR = 0;

	EEAR = addr;
	EEDR = value;

	EECR |= _BV(EEMPE);
	EECR |= _BV(EEPE);

	while (EECR & _BV(EEPE)) { }
}

static void read_temp()
{
	static int addr = 0;
	short temp, hum;

	if (!Tsensor1::read(hum, temp)) {
		led2::clear();
		temp = hum = 0x5a5a;
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

	write_eeprom(addr, 0xff);
	write_eeprom(addr+1, 0xff);
}

static void schedule()
{
	static short secs = 0;

	led::clear();

	if (++secs == 600) {
		read_temp();
		secs = 0;
	}

	led::set();

	WDTInterrupt::enqueue(schedule);
}

static void init()
{
	static char init_secs = 2;

	if (--init_secs) {
		WDTInterrupt::enqueue(init);
	} else {
		schedule();
	}
}

int main()
{
	PRR = ~_BV(PRTIM0);
	ACSR = _BV(ACD);
	DIDR0 = _BV(AIN1D) | _BV(AIN0D);

	led::mode(OUTPUT);
	led::set();

	led2::mode(OUTPUT);
	led2::set();

	WDTCR = _BV(WDP2) | _BV(WDP1);
	TCCR0B = _BV(CS01);

	sei();

	WDTInterrupt::enqueue(init);
	WDTInterrupt::loop();
}
