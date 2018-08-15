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

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <inttypes.h>
#include <string>

const uint64_t MOVE_STACK_SIZE = 8192;
const uint64_t GAME_STACK_SIZE = 1024;

enum piece { KING = 4, KNIGHT = 8, ROOK = 16, BISHOP = 32, QUEEN = 64, PAWN = 128 };

enum color { WHITE = 1, BLACK = 2 };

enum move_type { COLOR_PIECE = 0, PIECE_FROM = 1, PIECE_TO = 2, MOVE_TYPE = 3 };

enum en_passant_promotion_capturing
{ EN_PASSANT_LEFT = 1, EN_PASSANT_RIGHT = 2, CAPTURING = 4, GET_KNIGHT = 8, GET_ROOK = 16,
  GET_BISHOP = 32, GET_QUEEN = 64, GET_PAWN = 128 };

union move_t {
	uint8_t set[4];
	uint32_t move;
};

class MoveGen
{
public:
    static void add_move(const color c, const piece p, const uint8_t from, const uint8_t to);
	static void reset_move_stack();
	static std::string get_moves();
	static std::string get_legal_moves();
	static bool make_move(move_t &move);
	static void unmake_move();
	static bool white_to_move;
private:
	MoveGen();
    static uint64_t remove_unlegal_moves();
	static move_t move_stack[MOVE_STACK_SIZE];
	static move_t game_stack[GAME_STACK_SIZE];
    static uint64_t move_top;
    static uint64_t move_bottom;
	static int game_top;
};

#endif // MOVEGEN_H
