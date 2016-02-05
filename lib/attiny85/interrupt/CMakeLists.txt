# generated as: ../../../gen_int.sh /usr/lib/avr/include/avr/iotnx5.h
add_library("${MCU}_interrupt" STATIC
 INT0.cpp INT0_cb.cpp
 PCINT0.cpp PCINT0_cb.cpp
 TIM1_COMPA.cpp TIM1_COMPA_cb.cpp
 TIMER1_COMPA.cpp TIMER1_COMPA_cb.cpp
 TIM1_OVF.cpp TIM1_OVF_cb.cpp
 TIMER1_OVF.cpp TIMER1_OVF_cb.cpp
 TIM0_OVF.cpp TIM0_OVF_cb.cpp
 TIMER0_OVF.cpp TIMER0_OVF_cb.cpp
 EE_RDY.cpp EE_RDY_cb.cpp
 ANA_COMP.cpp ANA_COMP_cb.cpp
 ADC.cpp ADC_cb.cpp
 TIM1_COMPB.cpp TIM1_COMPB_cb.cpp
 TIMER1_COMPB.cpp TIMER1_COMPB_cb.cpp
 TIM0_COMPA.cpp TIM0_COMPA_cb.cpp
 TIMER0_COMPA.cpp TIMER0_COMPA_cb.cpp
 TIM0_COMPB.cpp TIM0_COMPB_cb.cpp
 TIMER0_COMPB.cpp TIMER0_COMPB_cb.cpp
 WDT.cpp WDT_cb.cpp
 USI_START.cpp USI_START_cb.cpp
 USI_OVF.cpp USI_OVF_cb.cpp
)
