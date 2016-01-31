#pragma once

#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>

typedef Radio<CC1101::CC1101<USI, Pin<Port<A>, 7>>> radio;

