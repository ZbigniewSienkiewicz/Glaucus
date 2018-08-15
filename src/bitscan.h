#ifndef BITSCAN_H
#define BITSCAN_H

#include <inttypes.h>

typedef uint64_t U64;

class Bitscan
{
public:
	static unsigned int get_lsb(U64);
private:
    Bitscan();
	 static const int index64[64];
};

#endif // BITSCAN_H
