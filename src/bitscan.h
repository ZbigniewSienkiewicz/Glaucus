#ifndef BITSCAN_H
#define BITSCAN_H

#include <inttypes.h>

class Bitscan
{
public:
	static uint8_t get_lsb(uint64_t);
private:
	Bitscan();
	static const uint8_t index64[64];
};

#endif // BITSCAN_H
