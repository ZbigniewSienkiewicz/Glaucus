/*
***************************************************************************
**
** Copyright (C) 2011 Zbigniew Sienkiewicz.
** All rights reserved.
**
** Glaucus is Glinski's hexagonal chess engine.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
***************************************************************************
*/

#ifndef HEXBITBOARD_H
#define HEXBITBOARD_H

#include <inttypes.h>
#include <string>
#include <iostream>

const unsigned int HEXES_NUMBER_ALIGN = 126;

enum hexes_std {
	HEX_A1=10, HEX_B1=11, HEX_C1=12, HEX_D1=13, HEX_E1=14, HEX_F1=15, HEX_G1=16, HEX_H1=17, HEX_I1=18, HEX_K1=19, HEX_L1=20,
	HEX_A2=21, HEX_B2=22, HEX_C2=23, HEX_D2=24, HEX_E2=25, HEX_F2=26, HEX_G2=27, HEX_H2=28, HEX_I2=29, HEX_K2=30, HEX_L2=31,
	HEX_A3=32, HEX_B3=33, HEX_C3=34, HEX_D3=35, HEX_E3=36, HEX_F3=37, HEX_G3=38, HEX_H3=39, HEX_I3=40, HEX_K3=41, HEX_L3=42,
	HEX_A4=43, HEX_B4=44, HEX_C4=45, HEX_D4=46, HEX_E4=47, HEX_F4=48, HEX_G4=49, HEX_H4=50, HEX_I4=51, HEX_K4=52, HEX_L4=53,
	HEX_A5=54, HEX_B5=55, HEX_C5=56, HEX_D5=57, HEX_E5=58, HEX_F5=59, HEX_G5=60, HEX_H5=61, HEX_I5=62, HEX_K5=63, HEX_L5=64,
	HEX_A6=65, HEX_B6=66, HEX_C6=67, HEX_D6=68, HEX_E6=69, HEX_F6=70, HEX_G6=71, HEX_H6=72, HEX_I6=73, HEX_K6=74, HEX_L6=75,
				  HEX_B7=77, HEX_C7=78, HEX_D7=79, HEX_E7=80, HEX_F7=81, HEX_G7=82, HEX_H7=83, HEX_I7=84, HEX_K7=85,
								 HEX_C8=89, HEX_D8=90, HEX_E8=91, HEX_F8=92, HEX_G8=93, HEX_H8=94, HEX_I8=95,
												HEX_D9=101,HEX_E9=102,HEX_F9=103,HEX_G9=104,HEX_H9=105,
															HEX_E10=113,HEX_F10=114,HEX_G10=115,
																			HEX_F11=125
};

enum men { EMPTY=0, WHITE_KING=1, WHITE_KNIGHT=2, WHITE_BISHOP=4, WHITE_ROOK=8, WHITE_QUEEN=16, WHITE_PAWN=32,
			  BLACK_KING=64, BLACK_KNIGHT=128, BLACK_BISHOP=256, BLACK_ROOK=512, BLACK_QUEEN=1024, BLACK_PAWN=2056 };

enum files { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_I, FILE_K, FILE_L };

enum ranks { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_9, RANK_10, RANK_11 };


struct bits128 {
    uint64_t lo;
	uint64_t hi;
	bits128() { lo=0; hi=0; }
	bits128(uint64_t l, uint64_t h) { lo = l; hi = h; }
	void set(unsigned int position)
	{
		if (position > 63) {
			hi |= (1ULL << (position-64));
		}
		else {
			lo |= (1ULL << position);
		}
	}
	void unset(unsigned int position)
	{
		if (position > 63) {
			hi &= ~(1ULL << (position-64));
		}
		else {
			lo &= ~(1ULL << position);
		}
	}
	bool is_set(unsigned int position)
	{
		if (position > 63) {
			if (hi & (1ULL << (position-64))) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (lo & (1ULL << position)) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	operator bool();
};

struct bitmaps {
	bits128 white_pieces;
	bits128 black_pieces;
	bits128 black_king;
	bits128 white_king;
	bits128 black_knight;
	bits128 white_knight;
};

bits128 operator<<(bits128 board, unsigned int val);
bits128* operator<<=(bits128 &board, unsigned int val);
bits128* operator|=(bits128 &board, bits128 mask);
bits128* operator&=(bits128 &board, bits128 mask);
bits128 operator&(bits128 board, bits128 mask);
bits128 operator|(bits128 board, bits128 mask);
bits128 operator~(bits128 board);

class Hexbitboard
{
public:
	static void init();
	static void new_game();
	static void clean_bitboards();
	static bool is_set(const unsigned int position);
	static bool is_set_white(const unsigned int position);
	static bool is_set_black(const unsigned int position);
	static bool is_set_white_king(const unsigned int position);
	static bool is_set_white_knight(const unsigned int position);
	static bool is_set_black_king(const unsigned int position);
	static bool is_set_black_knight(const unsigned int position);
	static void set_piece(const men piece, const unsigned int position);
	static void unset_all(const unsigned int position);
	static void set_white_king(const unsigned int position);
	static void set_black_king(const unsigned int position);
	static void set_white_knight(const unsigned int position);
	static void set_black_knight(const unsigned int position);
	static void unset_white_knight(const unsigned int position);
	static void unset_black_knight(const unsigned int position);
	static void set_white_black();
	static void set_white();
	static void set_black();
	static void set_white(bits128 mask) { bitboard.white_pieces |= mask; }
	static void set_black(bits128 mask) { bitboard.black_pieces |= mask; }
	static void set_empty(const unsigned int position);
	static bits128 white() { return bitboard.white_pieces; }
	static bits128 black() { return bitboard.black_pieces; }
	static bits128 white_king() { return bitboard.white_king; }
	static bits128 black_king() { return bitboard.black_king; }
	static bits128 white_knight() { return bitboard.white_knight; }
	static bits128 black_knight() { return bitboard.black_knight; }
	static std::string get_men(const unsigned int position);
	static bitmaps get_bitboards();
	static bool setup_board(const std::string fen);
	static bool hex_is_ok(const unsigned int file, const unsigned int rank);
	static unsigned int get_lsb_and_reset(bits128 &piece);
	static unsigned int get_lsb(bits128 &piece);
	static std::string pos_to_str(const unsigned int pos);
	static bool is_capture() { return (bitboard.white_pieces & bitboard.black_pieces); }
	static void backup_bitboards();
	static void restore_bitboards();
	static const int RANK_WIDTH;

private:
	Hexbitboard(); // so far private
	static const bits128 singlemask;
	static const bits128 zeromask;
	static bitmaps bitboard;
	static bitmaps bitboard_backup;
	static bool set_piece(const men piece, const unsigned int file, const unsigned int rank);
};


#endif // HEXBITBOARD_H
