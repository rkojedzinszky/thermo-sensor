#include "vcc.hpp"

void VCC::interrupt()
{
	done_ = true;
}

bool VCC::done_;
