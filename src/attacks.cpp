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
#include "attacks.h"
#include "movegen.h"

const bits128 Attacks::king_attacks[HEXES_NUMBER_ALIGN];
const bits128 Attacks::knight_attacks[HEXES_NUMBER_ALIGN];
bits128 Attacks::opponent_attacks;
bits128 Attacks::own_attacks;

Attacks::Attacks()
{
}

void Attacks::init()
{
    init_king_attacks(const_cast<bits128*> (king_attacks));
    init_knight_attacks(const_cast<bits128*> (knight_attacks));
    generate_own_attacks();
    generate_opponent_attacks();
}

void Attacks::init_king_attacks(bits128* king_attacks)
{
    for (uint64_t i = 0; i < HEXES_NUMBER_ALIGN; ++i) {
        king_attacks[i].lo = 0;
        king_attacks[i].hi = 0;
    }
    for (int32_t file = FILE_A; file <= FILE_L; ++file ) {
        for (int32_t rank = RANK_1; rank <= RANK_11; ++rank) {
            bits128 attack;
            if (!Hexbitboard::hex_is_ok(file, rank)) {
                continue;
            }
            if (Hexbitboard::hex_is_ok(file, rank+1)) {
                int32_t position = file + HEX_A1 + (rank+1)*Hexbitboard::RANK_WIDTH;
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file, rank-1)) {
                int32_t position = file + HEX_A1 + (rank-1)*Hexbitboard::RANK_WIDTH;
                if (position > 9) {
                    attack.set(position);
                }
            }
            int32_t left, right;
            if (file < FILE_F) {
                right = 1;
                left = 0;
            }
            else if (file > FILE_F) {
                right = 0;
                left = 1;
            }
            else {
                right = 0;
                left = 0;
            }
            for (int32_t row = rank-2; row <= rank+1; ++row) {
                if ((left+right) > 0) {
                    if (Hexbitboard::hex_is_ok(file-1, left+row)) {
                        int32_t position = abs(file-1 + HEX_A1 + (left+row)*Hexbitboard::RANK_WIDTH);
                        if (position > 9) {
                            attack.set(position);
                        }
                    }
                }
            }
            for (int row = rank-2; row <= rank+1; ++row) {
                if (Hexbitboard::hex_is_ok(file+1, right+row)) {
                    int32_t position = file+1 + HEX_A1 + (right+row)*Hexbitboard::RANK_WIDTH;
                    if (position > 9) {
                        attack.set(position);
                    }
                }
            }
            switch (file) {
            case FILE_A: case FILE_B: case FILE_C: case FILE_D:
                left = -1;
                right = 1;
                break;
            case FILE_E:
                left = -1;
                right = 0;
                break;
            case FILE_F:
                left = -1;
                right = -1;
                break;
            case FILE_G:
                left = 0;
                right = -1;
                break;
            case FILE_H: case FILE_I: case FILE_K: case FILE_L:
                left = 1;
                right = -1;
                break;
            }
            if (Hexbitboard::hex_is_ok(file-2, rank+left)) {
                int32_t position = file-2 + HEX_A1 + (rank+left)*Hexbitboard::RANK_WIDTH;
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+2, rank+right)) {
                int32_t position = file+2 + HEX_A1 + (rank+right)*Hexbitboard::RANK_WIDTH;
                if (position > 9) {
                    attack.set(position);
                }
            }
            int64_t pointer = file + HEX_A1 + rank*Hexbitboard::RANK_WIDTH;
            king_attacks[pointer] = attack;
        }
    }
}

void Attacks::init_knight_attacks(bits128* knight_attacks)
{
    for (uint64_t i = 0; i < HEXES_NUMBER_ALIGN; ++i) {
        knight_attacks[i].lo = 0;
        knight_attacks[i].hi = 0;
    }
    for (int file = FILE_A; file <= FILE_L; ++file ) {
        for (int rank = RANK_1; rank <= RANK_11; ++rank) {
            bits128 attack;
            if (!Hexbitboard::hex_is_ok(file, rank)) {
                continue;
            }
            int left1, right1;
            if (file < FILE_F) {
                right1 = 1;
                left1 = 0;
            }
            else if (file > FILE_F) {
                right1 = 0;
                left1 = 1;
            }
            else {
                right1 = 0;
                left1 = 0;
            }

            int left2, right2;
            if (file < FILE_E) {
                right2 = 2;
                left2 = 0;
            }
            else if (file < FILE_F) {
                right2 = 1;
                left2 = 0;
            }
            else if (file < FILE_G) {
                right2 = 0;
                left2 = 0;
            }
            else if (file < FILE_H) {
                right2 = 0;
                left2 = 1;
            }
            else {
                right2 = 0;
                left2 = 2;
            }

            int left3, right3;
            if (file < FILE_D) {
                right3 = 3;
                left3 = 0;
            }
            else if (file < FILE_E) {
                right3 = 2;
                left3 = 0;
            }
            else if (file < FILE_F) {
                right3 = 1;
                left3 = 0;
            }
            else if (file < FILE_G) {
                right3 = 0;
                left3 = 0;
            }
            else if (file < FILE_H) {
                right3 = 0;
                left3 = 1;
            }
            else if (file < FILE_I) {
                right3 = 0;
                left3 = 2;
            }
            else {
                right3 = 0;
                left3 = 3;
            }


            if (Hexbitboard::hex_is_ok(file-1, left1+rank+2)) {
                int position = (file-1 + HEX_A1 + (left1+rank+2)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file-1, left1+rank-3)) {
                int position = (file-1 + HEX_A1 + (left1+rank-3)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+1, right1+rank+2)) {
                int position = (file+1 + HEX_A1 + (right1+rank+2)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+1, right1+rank-3)) {
                int position = (file+1 + HEX_A1 + (right1+rank-3)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }

            if (Hexbitboard::hex_is_ok(file-2, left2+rank+1)) {
                int position = (file-2 + HEX_A1 + (left2+rank+1)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file-2, left2+rank-3)) {
                int position = (file-2 + HEX_A1 + (left2+rank-3)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+2, right2+rank+1)) {
                int position = (file+2 + HEX_A1 + (right2+rank+1)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+2, right2+rank-3)) {
                int position = (file+2 + HEX_A1 + (right2+rank-3)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }

            if (Hexbitboard::hex_is_ok(file-3, left3+rank-1)) {
                int position = (file-3 + HEX_A1 + (left3+rank-1)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file-3, left3+rank-2)) {
                int position = (file-3 + HEX_A1 + (left3+rank-2)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+3, right3+rank-1)) {
                int position = (file+3 + HEX_A1 + (right3+rank-1)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }
            if (Hexbitboard::hex_is_ok(file+3, right3+rank-2)) {
                int position = (file+3 + HEX_A1 + (right3+rank-2)*Hexbitboard::RANK_WIDTH);
                if (position > 9) {
                    attack.set(position);
                }
            }

            int32_t pointer = file + HEX_A1 + rank*Hexbitboard::RANK_WIDTH;
            knight_attacks[pointer] = attack;
        }
    }
}

void Attacks::generate_moves()
{
    bitmaps temp = Hexbitboard::get_bitboards();
    MoveGen::reset_move_stack();
    uint8_t pos_from, pos_to;

    if (MoveGen::white_to_move) {
        // white king attacks
        bits128 white_king = temp.white_king;
        pos_from = Hexbitboard::get_lsb(white_king);
        bits128 temp_attacks = king_attacks[pos_from] & ~(temp.white_pieces) &~(temp.black_king);
        while ((pos_to = Hexbitboard::get_lsb_and_reset(temp_attacks))) {
            assert(pos_to > 9);
            MoveGen::add_move(WHITE, KING, pos_from, pos_to);
        }

        // white knights attacks
        bits128 white_knight = temp.white_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(white_knight))) {
            temp_attacks = knight_attacks[pos_from] & ~(temp.white_pieces) & ~(temp.black_king);
            while ((pos_to = Hexbitboard::get_lsb_and_reset(temp_attacks))) {
                assert(pos_from > 9);
                assert(pos_to > 9);
                MoveGen::add_move(WHITE, KNIGHT, pos_from, pos_to);
            }
        }
    }
    else {
        // black king attacks
        bits128 black_king = temp.black_king;
        pos_from = Hexbitboard::get_lsb(black_king);
        bits128 temp_attacks = king_attacks[pos_from] & ~(temp.black_pieces) & ~(temp.white_king);
        while ((pos_to = Hexbitboard::get_lsb_and_reset(temp_attacks))) {
            MoveGen::add_move(BLACK, KING, pos_from, pos_to);
        }

        // black knights attacks
        bits128 black_knight = temp.black_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(black_knight))) {
            temp_attacks = knight_attacks[pos_from] & ~(temp.black_pieces) & ~(temp.white_king);
            while ((pos_to = Hexbitboard::get_lsb_and_reset(temp_attacks))) {
                MoveGen::add_move(BLACK, KNIGHT, pos_from, pos_to);
            }
        }
    }
}

void Attacks::generate_opponent_attacks()
{
    bitmaps temp = Hexbitboard::get_bitboards();
    uint8_t pos_from;

    if (!MoveGen::white_to_move) {
        // white king attacks
        bits128 white_king = temp.white_king;
        pos_from = Hexbitboard::get_lsb(white_king);
        bits128 temp_attacks = king_attacks[pos_from];
        opponent_attacks = temp_attacks;

        // white knights attacks
        bits128 white_knight = temp.white_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(white_knight))) {
            temp_attacks = knight_attacks[pos_from];
            opponent_attacks |= temp_attacks;
        }
    }
    else {
        // black king attacks
        bits128 black_king = temp.black_king;
        pos_from = Hexbitboard::get_lsb(black_king);
        bits128 temp_attacks = king_attacks[pos_from];
        opponent_attacks = temp_attacks;

        // black knights attacks
        bits128 black_knight = temp.black_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(black_knight))) {
            temp_attacks = knight_attacks[pos_from];
            opponent_attacks |= temp_attacks;
        }
    }
}

void Attacks::generate_own_attacks()
{
    bitmaps temp = Hexbitboard::get_bitboards();
    uint8_t pos_from;

    if (MoveGen::white_to_move) {
        // white king attacks
        bits128 white_king = temp.white_king;
        pos_from = Hexbitboard::get_lsb(white_king);
        bits128 temp_attacks = king_attacks[pos_from];
        own_attacks = temp_attacks;

        // white knights attacks
        bits128 white_knight = temp.white_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(white_knight))) {
            temp_attacks = knight_attacks[pos_from];
            own_attacks |= temp_attacks;
        }
    }
    else {
        // black king attacks
        bits128 black_king = temp.black_king;
        pos_from = Hexbitboard::get_lsb(black_king);
        bits128 temp_attacks = king_attacks[pos_from];
        own_attacks = temp_attacks;

        // black knights attacks
        bits128 black_knight = temp.black_knight;
        while ((pos_from = Hexbitboard::get_lsb_and_reset(black_knight))) {
            temp_attacks = knight_attacks[pos_from];
            own_attacks |= temp_attacks;
        }
    }
}

bool Attacks::position_is_ok()
{
    if (!Hexbitboard::get_black_king()) {
        return false;
    }
    if (!Hexbitboard::get_white_king()) {
        return false;
    }
    generate_own_attacks();
    if (MoveGen::white_to_move) {
        if (own_attacks & Hexbitboard::get_black_king()) {
            return false;
        }
    }
    else if (own_attacks & Hexbitboard::get_white_king()) {
        return false;
    }
    return true;
}

bool Attacks::king_is_attacked()
{
    assert(position_is_ok());
    generate_opponent_attacks();
    if (MoveGen::white_to_move) {
        return (opponent_attacks & Hexbitboard::get_white_king());
    }
    else {
        return (opponent_attacks & Hexbitboard::get_black_king());
    }
}
