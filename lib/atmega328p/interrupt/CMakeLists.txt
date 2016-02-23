# generated as: ../../../gen_int.sh /usr/lib/avr/include/avr/iom328p.h
add_library("${MCU}_interrupt" STATIC
 INT0.cpp INT0_cb.cpp
 INT1.cpp INT1_cb.cpp
 PCINT0.cpp PCINT0_cb.cpp
 PCINT1.cpp PCINT1_cb.cpp
 PCINT2.cpp PCINT2_cb.cpp
 WDT.cpp WDT_cb.cpp
 TIMER2_COMPA.cpp TIMER2_COMPA_cb.cpp
 TIMER2_COMPB.cpp TIMER2_COMPB_cb.cpp
 TIMER2_OVF.cpp TIMER2_OVF_cb.cpp
 TIMER1_CAPT.cpp TIMER1_CAPT_cb.cpp
 TIMER1_COMPA.cpp TIMER1_COMPA_cb.cpp
 TIMER1_COMPB.cpp TIMER1_COMPB_cb.cpp
 TIMER1_OVF.cpp TIMER1_OVF_cb.cpp
 TIMER0_COMPA.cpp TIMER0_COMPA_cb.cpp
 TIMER0_COMPB.cpp TIMER0_COMPB_cb.cpp
 TIMER0_OVF.cpp TIMER0_OVF_cb.cpp
 SPI_STC.cpp SPI_STC_cb.cpp
 USART_RX.cpp USART_RX_cb.cpp
 USART_UDRE.cpp USART_UDRE_cb.cpp
 USART_TX.cpp USART_TX_cb.cpp
 ADC.cpp ADC_cb.cpp
 EE_READY.cpp EE_READY_cb.cpp
 ANALOG_COMP.cpp ANALOG_COMP_cb.cpp
 TWI.cpp TWI_cb.cpp
 SPM_READY.cpp SPM_READY_cb.cpp
)
