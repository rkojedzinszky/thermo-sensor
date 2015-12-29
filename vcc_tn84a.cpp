#include "vcc_tn84a.hpp"

void VCC_tn84a::interrupt()
{
	done_ = true;
}

bool VCC_tn84a::done_;
