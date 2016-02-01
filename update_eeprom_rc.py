#!/usr/bin/env python

REGISTERS = {
    'IOCFG2': 0x00,
    'IOCFG1': 0x01,
    'IOCFG0': 0x02,
    'FIFOTHR': 0x03,
    'SYNC1': 0x04,
    'SYNC0': 0x05,
    'PKTLEN': 0x06,
    'PKTCTRL1': 0x07,
    'PKTCTRL0': 0x08,
    'ADDR': 0x09,
    'CHANNR': 0x0A,
    'FSCTRL1': 0x0B,
    'FSCTRL0': 0x0C,
    'FREQ2': 0x0D,
    'FREQ1': 0x0E,
    'FREQ0': 0x0F,
    'MDMCFG4': 0x10,
    'MDMCFG3': 0x11,
    'MDMCFG2': 0x12,
    'MDMCFG1': 0x13,
    'MDMCFG0': 0x14,
    'DEVIATN': 0x15,
    'MCSM2': 0x16,
    'MCSM1': 0x17,
    'MCSM0': 0x18,
    'FOCCFG': 0x19,
    'BSCFG': 0x1A,
    'AGCCTRL2': 0x1B,
    'AGCCTRL1': 0x1C,
    'AGCCTRL0': 0x1D,
    'WOREVT1': 0x1E,
    'WOREVT0': 0x1F,
    'WORCTRL': 0x20,
    'FREND1': 0x21,
    'FREND0': 0x22,
    'FSCAL3': 0x23,
    'FSCAL2': 0x24,
    'FSCAL1': 0x25,
    'FSCAL0': 0x26,
    'RCCTRL1': 0x27,
    'RCCTRL0': 0x28,
    'FSTEST': 0x29,
    'PTEST': 0x2A,
    'AGCTEST': 0x2B,
    'TEST2': 0x2C,
    'TEST1': 0x2D,
    'TEST0': 0x2E,
    'PATABLE': 0x3E,
    }

if __name__ == '__main__':
    import sys
    import re

    with open('eeprom', 'r+b') as fh:
        fh.seek(20)

        for line in sys.stdin:
            if re.match('^\s*#', line):
                continue

            m = re.match('(?P<reg>\w+)\s+(?P<value>[0-9a-fA-F]+)', line)
            if not m:
                continue

            m = m.groupdict()
            fh.write(chr(REGISTERS[m['reg']]))
            fh.write(chr(int(m['value'], 16)))

        fh.write(b"\xff" * (512 - fh.tell()))
