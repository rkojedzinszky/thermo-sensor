# generated as: ../../../gen_int.sh /usr/lib/avr/include/avr/iotn84a.h
add_library("${MCU}_interrupt" STATIC
 EXT_INT0.cpp
 PCINT0.cpp
 PCINT1.cpp
 WATCHDOG.cpp
 TIM1_CAPT.cpp
 TIM1_COMPA.cpp
 TIM1_COMPB.cpp
 TIM1_OVF.cpp
 TIM0_COMPA.cpp
 TIM0_COMPB.cpp
 TIM0_OVF.cpp
 ANA_COMP.cpp
 ADC.cpp
 EE_RDY.cpp
 USI_STR.cpp
 USI_OVF.cpp
)
