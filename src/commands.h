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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include "hexbitboard.h"


struct command {
	std::string name;
	void (*function)();
	std::string description;
};

class Commands
{
public:
	static void command_init();
    [[ noreturn ]] static void read_commands();
private:
	static void command_display();
    [[ noreturn ]] static void command_quit() ;
	static void command_help();
	static void command_new();
	static void command_edit();
	static void command_rotate();
	static void command_fen();
	static void command_white();
	static void command_black();
	static void command_moves();
    static void command_attacks();
    static std::string recode_display(std::string hexboard_display);
    static std::string recode_attacks(const std::string hexboard_display);
    static void edit();
	static const std::string ENGINE_NAME;
	static const command command_list[];
	static const std::string hexboard_display_normal;
	static const std::string hexboard_display_rotate;
	static bool rotate;

	Commands(); // this is a static class
};

#endif // COMMANDS_H
