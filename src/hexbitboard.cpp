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

#include <iomanip>
#include <sstream>
#include <cassert>
#include "hexbitboard.h"
#include "utils.h"
#include "bitscan.h"

using namespace std;

bitmaps Hexbitboard::bitboard;
bitmaps Hexbitboard::bitboard_backup;
const bits128 Hexbitboard::singlemask(1ULL,0ULL);
const bits128 Hexbitboard::zeromask(0ULL,0ULL);
const int Hexbitboard::RANK_WIDTH = 11;

Hexbitboard::Hexbitboard()
{
}

void Hexbitboard::init()
{
	new_game();
}

void Hexbitboard::new_game()
{
	setup_board("///N7n///K8k/N7n");
}

void Hexbitboard::clean_bitboards()
{
	bitboard.white_king = zeromask;
	bitboard.black_king = zeromask;
	bitboard.white_knight = zeromask;
	bitboard.black_knight = zeromask;
	bitboard.white_pieces = zeromask;
	bitboard.black_pieces = zeromask;
}

bool Hexbitboard::setup_board(string xfen)
{
	unsigned int file = 0;
	unsigned int rank = 0;
	unsigned int buffer = 0;
	int step;
	men piece = EMPTY;
	backup_bitboards();
	clean_bitboards();

	int xfen_length = xfen.length();
	bool xfen_good = true;

	for (int i = 0; i < xfen_length; ++i) {
		switch (xfen[i]) {
		case 'K':
			piece = WHITE_KING;
			break;
		case 'N':
			piece = WHITE_KNIGHT;
			break;
		case 'k':
			piece = BLACK_KING;
			break;
		case 'n':
			piece = BLACK_KNIGHT;
			break;
		case '/':
			file++;
			rank = 0;
			buffer = 0;
			continue;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			step = (xfen[i] - '0');
			if (!buffer) {
				buffer = step;
			}
			else {
				buffer = buffer*10 + step;
			}
			break;
		default:
			cout << "xfen error\n";
			restore_bitboards();
			return false;
		}
		if (piece != EMPTY) {
			if (buffer) {
				rank += buffer;
			}
			xfen_good = set_piece(piece, file, rank);
			if (!xfen_good) {
				break;
			}
			piece = EMPTY;
			rank++;
			buffer = 0;
		}
	}
	if (!xfen_good) {
		cout << "xfen error\n";
		restore_bitboards();
		return false;
	}
	set_white_black();
	return true;
}

bool Hexbitboard::hex_is_ok(const unsigned int file, const unsigned int rank)
{
	if ((file > 10) || (rank > 10)) {
		return false;
	}
	switch (file) {
	case FILE_A: case FILE_L:
		if (rank > 5) {
			return false;
		}
        break;
    case FILE_B: case FILE_K:
		if (rank > 6) {
			return false;
		}
        break;
	case FILE_C: case FILE_I:
		if (rank > 7) {
			return false;
		}
        break;
	case FILE_D: case FILE_H:
		if (rank > 8) {
			return false;
		}
        break;
	case FILE_E: case FILE_G:
		if (rank > 9) {
			return false;
		}
        break;
	default:
		if (rank > 10) {
			return false;
		}
	}
	return true;
}


void Hexbitboard::restore_bitboards()
{
	bitboard = bitboard_backup;
}

void Hexbitboard::backup_bitboards()
{
	bitboard_backup = bitboard;
}

bitmaps Hexbitboard::get_bitboards()
{
	return bitboard;
}

bool Hexbitboard::is_set(const unsigned int position)
{
	if ((bitboard.black_pieces & (singlemask << position)) || (bitboard.white_pieces & (singlemask << position))) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_white(const unsigned int position)
{
	if (bitboard.white_pieces & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_black(const unsigned int position)
{
	if (bitboard.black_pieces & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_white_king(const unsigned int position)
{
	if (bitboard.white_king & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_black_king(const unsigned int position)
{
	if (bitboard.black_king & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_white_knight(const unsigned int position)
{
	if (bitboard.white_knight & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::is_set_black_knight(const unsigned int position)
{
	if (bitboard.black_knight & (singlemask << position)) {
		return true;
	}
	else {
		return false;
	}
}

bool Hexbitboard::set_piece(const men piece, const unsigned int file, const unsigned int rank)
{
	if (!hex_is_ok(file, rank)) {
		return false;
	}

	int pos = (file + HEX_A1) + rank * RANK_WIDTH;
	set_piece(piece, pos);
	return true;
}

void Hexbitboard::set_piece(const men piece, const unsigned int position)
{
	switch (piece) {
	case EMPTY:
		unset_all(position);
		break;
	case WHITE_KING:
		unset_all(position);
		set_white_king(position);
		break;
	case BLACK_KING:
		unset_all(position);
		set_black_king(position);
		break;
	case WHITE_KNIGHT:
		unset_all(position);
		set_white_knight(position);
		break;
	case BLACK_KNIGHT:
		unset_all(position);
		set_black_knight(position);
		break;
	default:
		assert(false);
		break;
	}
}

void Hexbitboard::unset_all(const unsigned int position)
{
	bits128 mask = ~(singlemask << position);
	bitboard.white_king &= mask;
	bitboard.white_knight &= mask;
	bitboard.black_king &= mask;
	bitboard.black_knight &= mask;
	bitboard.white_pieces &= mask;
	bitboard.white_pieces &= mask;
}

void Hexbitboard::set_white_king(const unsigned int position)
{
	bitboard.white_king = (singlemask << position);
}

void Hexbitboard::set_black_king(const unsigned int position)
{
	bitboard.black_king = (singlemask << position);
}

void Hexbitboard::set_white_knight(const unsigned int position)
{
	bitboard.white_knight |= (singlemask << position);
}

void Hexbitboard::set_black_knight(const unsigned int position)
{
	bitboard.black_knight |= (singlemask << position);
}

void Hexbitboard::unset_white_knight(const unsigned int position)
{
	bitboard.white_knight &= ~(singlemask << position);
}

void Hexbitboard::unset_black_knight(const unsigned int position)
{
	bitboard.black_knight &= ~(singlemask << position);
}

void Hexbitboard::set_white_black()
{
	bitboard.white_pieces = bitboard.white_king | bitboard.white_knight;
	bitboard.black_pieces = bitboard.black_king | bitboard.black_knight;
}

void Hexbitboard::set_white()
{
	bitboard.white_pieces = bitboard.white_king | bitboard.white_knight;
}

void Hexbitboard::set_black()
{
	bitboard.black_pieces = bitboard.black_king | bitboard.black_knight;
}

void Hexbitboard::set_empty(const unsigned int position)
{
	bits128 mask = ~(singlemask << position);
	if (mask & bitboard.black_pieces) {
		bitboard.black_king &= mask;
		bitboard.black_knight &= mask;
		bitboard.black_pieces &= mask;
	}
	else if (mask & bitboard.white_pieces) {
		bitboard.white_king &= mask;
		bitboard.white_knight &= mask;
		bitboard.white_pieces &= mask;
	}

}

std::string Hexbitboard::get_men(const unsigned int position)
{
	bits128 test = (singlemask << position);
	if (bitboard.white_pieces & test) {
		if (bitboard.white_king & test) {
			return "K_";
		}
		else if (bitboard.white_knight & test) {
			return "N_";
		}
		else {
			return "??";
		}
	}
	else if (bitboard.black_pieces & test) {
		if (bitboard.black_king & test) {
			return "K*";
		}
		else if (bitboard.black_knight & test) {
			return "N*";
		}
		else {
			return "??";
		}
	}
	return "__";
}

unsigned int Hexbitboard::get_lsb_and_reset(bits128 &piece)
{
	unsigned int pos;
	if (!piece) {
		return 0;
	}
	else if (piece.lo && (pos = Bitscan::get_lsb(piece.lo))) {
		piece.lo &= (piece.lo-1);
		return pos;
	}
	else if (piece.hi && (pos = Bitscan::get_lsb(piece.hi))) {
		piece.hi &= (piece.hi-1);
		return pos+64;
	}
	return 0;
}

unsigned int Hexbitboard::get_lsb(bits128 &piece)
{
	unsigned int pos;
	if (!piece) {
		return 0;
	}
	else if (piece.lo && (pos = Bitscan::get_lsb(piece.lo))) {
		return pos;
	}
	else if (piece.hi && (pos = Bitscan::get_lsb(piece.hi))) {
		return pos+64;
	}
	return 0;
}

std::string Hexbitboard::pos_to_str(const unsigned int pos)
{
	int rank = (pos - 10) / 11;
	int file = (pos - 10) % 11;
	char f = file + int('a');
	if (f > 'i') f++;
	ostringstream field;
	field << f << (rank+1);
	return field.str();
}

bits128 operator<<(bits128 board, unsigned int val)
{
	if (val >= 64) {
		board.hi = board.lo << (val-64);
		board.lo = 0;
	}
	else {
		board.hi = (board.hi << val) | (board.lo >> (64 - val));
		board.lo = board.lo << val;
	}
	return board;
}

bits128* operator<<=(bits128 &board, unsigned int val)
{
	if (val >= 64) {
		board.hi = board.lo << (val-64);
		board.lo = 0;
	}
	else {
		board.hi = (board.hi << val) | (board.lo >> (64 - val));
		board.lo = board.lo << val;
	}
	return &board;
}

bits128* operator&=(bits128 &board, bits128 mask)
{
	board.lo &= mask.lo;
	board.hi &= mask.hi;
	return &board;
}

bits128 operator&(bits128 board, bits128 mask)
{
	board.lo &= mask.lo;
	board.hi &= mask.hi;
	return board;
}

bits128 operator|(bits128 board, bits128 mask)
{
	board.lo |= mask.lo;
	board.hi |= mask.hi;
	return board;
}

bits128* operator|=(bits128 &board, bits128 mask)
{
	board.lo |= mask.lo;
	board.hi |= mask.hi;
	return &board;
}

bits128 operator~(bits128 board)
{
	board.lo = ~board.lo;
	board.hi = ~board.hi;
	return board;
}

bits128::operator bool()
{
	if ((lo) || (hi)) {
		return true;
	}
	else {
		return false;
	}
}
