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

#include <iostream>
#include "utils.h"

using namespace std;

static void binary(const uint64_t number, uint64_t &pos, string &bin);

void Utils::to_binary(const uint64_t bitmap)
{
	string bin = "0000000000000000000000000000000000000000000000000000000000000000";
    uint64_t pos = bin.length() - 1;
	binary(bitmap, pos, bin);
	cout << '\t' << bin << endl;
}

void Utils::to_binary(const uint64_t bitmap, const string label)
{
	string bin = "0000000000000000000000000000000000000000000000000000000000000000";
    uint64_t pos = bin.length() - 1;
	binary(bitmap, pos, bin);
	cout << label << "->\t" << bin << endl;
}

void Utils::to_binary(const bits128 bitmap, const string label)
{
	string bin = "0000000000000000000000000000000000000000000000000000000000000000";
    uint64_t pos = bin.length() - 1;
	binary(bitmap.lo, pos, bin);
	cout << label << ".lo->\t" << bin << endl;
	bin = "0000000000000000000000000000000000000000000000000000000000000000";
	pos = bin.length() - 1;
	binary(bitmap.hi, pos, bin);
	cout << label << ".hi->\t" << bin << endl;
}


void binary(const uint64_t number, uint64_t &pos, string &bin)
{
	uint64_t remainder;

	if(number == 1) {
		bin[pos] = '1';
		return;
	}
	else if (number == 0) {
		bin[pos] = '0';
		return;
	}

	remainder = number%2;
	bin[pos] = (remainder==1) ? '1' : '0';
	pos--;
	binary(number >> 1, pos, bin);
}

