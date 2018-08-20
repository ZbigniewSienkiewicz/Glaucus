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

#ifndef ATTACKS_H
#define ATTACKS_H

#include "hexbitboard.h"

const uint32_t HEXES_NUMBER_MAX = 126;

class Attacks
{
public:
	static const bits128 king_attacks[HEXES_NUMBER_MAX];
	static const bits128 knight_attacks[HEXES_NUMBER_MAX];
	static void init();
    static void generate_moves();
	static void generate_opponent_attacks();
	static void generate_own_attacks();
	static bool position_is_ok();
	static bool king_is_attacked();
	static bits128 enemy_attacks() { return opponent_attacks; }
	static bits128 my_attacks() { return own_attacks; }
private:
	 Attacks();
	 static bits128 opponent_attacks;
	 static bits128 own_attacks;
};

#endif // ATTACKS_H
