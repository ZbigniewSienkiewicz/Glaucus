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

#include <cassert>
#include <sstream>
#include "movegen.h"
#include "hexbitboard.h"
#include "attacks.h"
#include "utils.h"

using std::cout;
using std::endl;

uint64_t MoveGen::move_top = 0;
uint64_t MoveGen::move_bottom = 0;
int MoveGen::game_top = 0;
bool MoveGen::white_to_move = true;

move_t MoveGen::move_stack[MOVE_STACK_SIZE];
move_t MoveGen::game_stack[GAME_STACK_SIZE];

MoveGen::MoveGen()
{
}

void MoveGen::add_move(const color c, const piece p, const uint8_t from, const uint8_t to)
{
	assert(move_top < MOVE_STACK_SIZE);
    assert(from > 9);
    assert(to > 9);

	move_stack[move_top].set[COLOR_PIECE] = c | p;
	move_stack[move_top].set[PIECE_FROM] = from;
	move_stack[move_top].set[PIECE_TO] = to;
	move_stack[move_top].set[MOVE_TYPE] = 0;
	move_top++;
}

void MoveGen::reset_move_stack()
{
	move_top = 0;
	move_bottom = 0;
}

std::string MoveGen::get_moves()
{
	std::ostringstream list;
    for (uint64_t i = move_bottom; i < move_top; ++i) {
		switch (move_stack[i].set[COLOR_PIECE] & 0xFCU) {  // unset color bits
		case KING:
			list << " K";
			break;
		case KNIGHT:
			list << " N";
			break;
		default:
			list << " ?";
		}
		list << Hexbitboard::pos_to_str(move_stack[i].set[1]);
		list << Hexbitboard::pos_to_str(move_stack[i].set[2]);
	}

	list << "\ntotal number of moves = " << (move_top - move_bottom);
	list << "\n";
	return list.str();
}

std::string MoveGen::get_legal_moves()
{
    uint64_t legal_moves = remove_unlegal_moves();
	if (legal_moves) {
		return get_moves();
	}
	else {
		if (Attacks::king_is_attacked()) {
			return "king is mated\n";
		}
		else {
			return "king is stalemated\n";
		}
	}
}

uint64_t MoveGen::remove_unlegal_moves()
{
    for (uint64_t i = move_bottom; i < move_top; ++i) {
		if (!make_move(move_stack[i])) {
			unmake_move();
			move_stack[i] = move_stack[--move_top];
			i--;
		}
		else {
			unmake_move();
		}
	}
	return (move_top - move_bottom);
}

bool MoveGen::make_move(move_t &move)
{
	// update bitboard piece from
	// update bitboard piece to (if capture set capture field and captured piece)
    //cout << "making move " << Hexbitboard::pos_to_str(move.set[PIECE_FROM])
    //	  << Hexbitboard::pos_to_str(move.set[PIECE_TO]) << endl;
	if (move.set[COLOR_PIECE] & WHITE) {
		switch (move.set[COLOR_PIECE] & 0xFCU) { // unset color bits
		case KING:
            //cout << " +K" << Hexbitboard::pos_to_str(move.set[PIECE_TO]);
			Hexbitboard::set_white_king(move.set[PIECE_TO]);
			break;
		case KNIGHT:
            //cout << " -N" << Hexbitboard::pos_to_str(move.set[PIECE_FROM]);
			Hexbitboard::unset_white_knight(move.set[PIECE_FROM]);
            //cout << " +N" << Hexbitboard::pos_to_str(move.set[PIECE_TO]);
			Hexbitboard::set_white_knight(move.set[PIECE_TO]);
			break;
		default:
			assert(false);
			break;
		}
        //cout << endl;
		Hexbitboard::set_white();
		if (Hexbitboard::is_capture()) {
            //cout << "\tmove is capture" << endl;
			if (Hexbitboard::white() & Hexbitboard::black_knight()) {
                //cout << "\t -N*" << Hexbitboard::pos_to_str(move.set[PIECE_TO]);
				Hexbitboard::unset_black_knight(move.set[PIECE_TO]);
				move.set[MOVE_TYPE] |= (CAPTURING | GET_KNIGHT);
			}
			Hexbitboard::set_black();
            //cout << endl;
		}
		game_stack[game_top++] = move;
		Attacks::generate_opponent_attacks();
		white_to_move = !white_to_move;
		if (Hexbitboard::white_king() & Attacks::enemy_attacks()) {
			cout << "\tmove is illegal" << endl;
			return false;
		}
		else {
			return true;
		}
	}
	else {
		switch (move.set[COLOR_PIECE] & 0xFCU) { // unset color bits
		case KING:
			Hexbitboard::set_black_king(move.set[PIECE_TO]);
            break;
		case KNIGHT:
			Hexbitboard::unset_black_knight(move.set[PIECE_FROM]);
			Hexbitboard::set_black_knight(move.set[PIECE_TO]);
			break;
		default:
			assert(false);
			break;
		}
		Hexbitboard::set_black();
		if (Hexbitboard::is_capture()) {
			if (Hexbitboard::black() & Hexbitboard::white_knight()) {
				Hexbitboard::unset_white_knight(move.set[PIECE_TO]);
				move.set[MOVE_TYPE] |= (CAPTURING | GET_KNIGHT);
			}
			Hexbitboard::set_white();
		}
		game_stack[game_top++] = move;
		Attacks::generate_opponent_attacks();
		white_to_move = !white_to_move;
		if (Hexbitboard::black_king() & Attacks::enemy_attacks()) {
			return false;
		}
		else {
			return true;
		}
	}
}

void MoveGen::unmake_move()
{
	assert(game_top > 0);
	move_t move = game_stack[--game_top];
	// update bitboard piece from
	// update bitboard piece to (if capture set capture field and captured piece)
	if (move.set[COLOR_PIECE] & WHITE) {
        //cout << "unmaking move " << Hexbitboard::pos_to_str(move.set[PIECE_FROM])
        //	  << Hexbitboard::pos_to_str(move.set[PIECE_TO]) << endl;
		switch (move.set[COLOR_PIECE] & 0xFCU) { // unset color bits
		case KING:
            //cout << " +K" << Hexbitboard::pos_to_str(move.set[PIECE_FROM]);
			Hexbitboard::set_white_king(move.set[PIECE_FROM]); // restore king position
			break;
		case KNIGHT:
            //cout << " -N" << Hexbitboard::pos_to_str(move.set[PIECE_TO]);
			Hexbitboard::unset_white_knight(move.set[PIECE_TO]);
            //cout << " +N" << Hexbitboard::pos_to_str(move.set[PIECE_FROM]);
			Hexbitboard::set_white_knight(move.set[PIECE_FROM]); // restore knight position
			break;
		default:
			assert(false);
			break;
		}
        //cout << endl;
		Hexbitboard::set_white();
		if (move.set[MOVE_TYPE] & CAPTURING) {
			if (move.set[MOVE_TYPE] & GET_KNIGHT) {
                //cout << " +N*" << Hexbitboard::pos_to_str(move.set[PIECE_TO]);
				Hexbitboard::set_black_knight(move.set[PIECE_TO]); // restore captured knight
			}
			Hexbitboard::set_black();
            //cout << endl;
		}
	}
	else {
		switch (move.set[COLOR_PIECE] & 0xFCU) { // unset color bits
		case KING:
			Hexbitboard::set_black_king(move.set[PIECE_FROM]); // restore king position
			break;
		case KNIGHT:
			Hexbitboard::unset_black_knight(move.set[PIECE_TO]);
			Hexbitboard::set_black_knight(move.set[PIECE_FROM]); // restore knight position
			break;
		default:
			assert(false);
			break;
		}
		Hexbitboard::set_black();
		if (move.set[MOVE_TYPE] & CAPTURING) {
			if (move.set[MOVE_TYPE] & GET_KNIGHT) {
				Hexbitboard::set_white_knight(move.set[PIECE_TO]); // restore captured knight
			}
			Hexbitboard::set_white();
		}
	}
	white_to_move = !white_to_move;
}
