#include "sensor.hpp"

template uint8_t CC1101::CC1101<USI, Pin<Port<B>, 3>>::select();
template bool AM2302<Pin<Port<B>, 4>>::read(short&, short&);
