#include "vcc_tnx5.hpp"

void VCC_tnx5::interrupt()
{
	done_ = true;
}

bool VCC_tnx5::done_;
