#pragma once

#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>
#include <am2302.hpp>

typedef AM2302<Pin<Port<B>, 4>> am2302;
typedef Radio<CC1101::CC1101<USI, Pin<Port<B>, 3>>> radio;

