# generated as: ../../../gen_int.sh /usr/lib/avr/include/avr/iotn84a.h
add_library("${MCU}_interrupt" STATIC
 EXT_INT0.cpp EXT_INT0_cb.cpp
 PCINT0.cpp PCINT0_cb.cpp
 PCINT1.cpp PCINT1_cb.cpp
 WATCHDOG.cpp WATCHDOG_cb.cpp
 TIM1_CAPT.cpp TIM1_CAPT_cb.cpp
 TIM1_COMPA.cpp TIM1_COMPA_cb.cpp
 TIM1_COMPB.cpp TIM1_COMPB_cb.cpp
 TIM1_OVF.cpp TIM1_OVF_cb.cpp
 TIM0_COMPA.cpp TIM0_COMPA_cb.cpp
 TIM0_COMPB.cpp TIM0_COMPB_cb.cpp
 TIM0_OVF.cpp TIM0_OVF_cb.cpp
 ANA_COMP.cpp ANA_COMP_cb.cpp
 ADC.cpp ADC_cb.cpp
 EE_RDY.cpp EE_RDY_cb.cpp
 USI_STR.cpp USI_STR_cb.cpp
 USI_OVF.cpp USI_OVF_cb.cpp
)
