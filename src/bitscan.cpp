#include <cassert>
#include "bitscan.h"

const int Bitscan::index64[64] = {
	63,  0, 58,  1, 59, 47, 53,  2,
	60, 39, 48, 27, 54, 33, 42,  3,
	61, 51, 37, 40, 49, 18, 28, 20,
	55, 30, 34, 11, 43, 14, 22,  4,
	62, 57, 46, 52, 38, 26, 32, 41,
	50, 36, 17, 19, 29, 10, 13, 21,
	56, 45, 25, 31, 35, 16,  9, 12,
	44, 24, 15,  8, 23,  7,  6,  5
};

Bitscan::Bitscan()
{
}

/**
 * bitScanForward
 * @author Matt Taylor
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
unsigned int Bitscan::get_lsb(U64 bb)
{
	const U64 debruijn64 = 0x07EDD5E59A4E28C2ULL;
	assert (bb != 0);
	return index64[((bb & -bb) * debruijn64) >> 58];
}