#include "sensor.hpp"

template uint8_t CC1101::CC1101<USI, Pin<Port<B>, 3>>::select();
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::write_txfifo(uint8_t* values, uint8_t len);
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::read_rxfifo(uint8_t* values, uint8_t len);
template bool AM2302<Pin<Port<B>, 4>>::read(short&, short&);
