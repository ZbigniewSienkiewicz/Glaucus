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

#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "commands.h"
#include "hexbitboard.h"
#include "attacks.h"
#include "movegen.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

const string Commands::ENGINE_NAME = "Glaucus, Glinski's hexagonal chess engine\nCopyright(c) by Zbigniew Sienkiewicz, 2011\ntype 'help' for list of commands\n";

const string Commands::hexboard_display_normal =
	"\n              11  __  11                 \n"
	"           10  __/  \\__  10               \n"
	"         9  __/. \\FK/. \\__  9            \n"
	"      8  __/+ \\EJ/+ \\GJ/+ \\__  8        \n"
	"   7  __/  \\DI/  \\FJ/  \\HI/  \\__  7    \n"
	"   __/. \\CH/. \\EI/. \\GI/. \\IH/. \\__   \n"
	" 6/+ \\BG/+ \\DH/+ \\FI/+ \\HH/+ \\KG/+ \\6\n"
	"  \\AF/  \\CG/  \\EH/  \\GH/  \\IG/  \\LF/ \n"
	" 5/. \\BF/. \\DG/. \\FH/. \\HG/. \\KF/. \\5\n"
	"  \\AE/+ \\CF/+ \\EG/+ \\GG/+ \\IF/+ \\LE/ \n"
	" 4/  \\BE/  \\DF/  \\FG/  \\HF/  \\KE/  \\4\n"
	"  \\AD/. \\CE/. \\EF/. \\GF/. \\IE/. \\LD/ \n"
	" 3/+ \\BD/+ \\DE/+ \\FF/+ \\HE/+ \\KD/+ \\3\n"
	"  \\AC/  \\CD/  \\EE/  \\GE/  \\ID/  \\LC/ \n"
	" 2/. \\BC/. \\DD/. \\FE/. \\HD/. \\KC/. \\2\n"
	"  \\AB/+ \\CC/+ \\ED/+ \\GD/+ \\IC/+ \\LB/ \n"
	" 1/  \\BB/  \\DC/  \\FD/  \\HC/  \\KB/  \\1\n"
	"  \\AA/. \\CB/. \\EC/. \\GC/. \\IB/. \\LA/ \n"
	"   a \\BA/+ \\DB/+ \\FC/+ \\HB/+ \\KA/l    \n"
	"      b \\CA/  \\EB/  \\GB/  \\IA/k        \n"
	"         c \\DA/. \\FB/. \\HA/i            \n"
	"            d \\EA/+ \\GA/h                \n"
	"               e \\FA/g                    \n"
	"                  f                        \n";

const string Commands::hexboard_display_rotate =
	"\n                  f                      \n"
	"               g  __  e                    \n"
	"            h  __/+ \\__  d                \n"
	"         i  __/. \\FA/. \\__  c            \n"
	"      k  __/  \\GA/  \\EA/  \\__  b        \n"
	"   l  __/+ \\HA/+ \\FB/+ \\DA/+ \\__  a    \n"
	"   __/. \\IA/. \\GB/. \\EB/. \\CA/. \\__   \n"
	"  /  \\KA/  \\HB/  \\FC/  \\DB/  \\BA/  \\ \n"
	" 1\\LA/+ \\IB/+ \\GC/+ \\EC/+ \\CB/+ \\AA/1\n"
	"  /. \\KB/. \\HC/. \\FD/. \\DC/. \\BB/. \\ \n"
	" 2\\LB/  \\IC/  \\GD/  \\ED/  \\CC/  \\AB/2\n"
	"  /+ \\KC/+ \\HD/+ \\FE/+ \\DD/+ \\BC/+ \\ \n"
	" 3\\LC/. \\ID/. \\GE/. \\EE/. \\CD/. \\AC/3\n"
	"  /  \\KD/  \\HE/  \\FF/  \\DE/  \\BD/  \\ \n"
	" 4\\LD/+ \\IE/+ \\GF/+ \\EF/+ \\CE/+ \\AD/4\n"
	"  /. \\KE/. \\HF/. \\FG/. \\DF/. \\BE/. \\ \n"
	" 5\\LE/  \\IF/  \\GG/  \\EG/  \\CF/  \\AE/5\n"
	"  /+ \\KF/+ \\HG/+ \\FH/+ \\DG/+ \\BF/+ \\ \n"
	" 6\\LF/. \\IG/. \\GH/. \\EH/. \\CG/. \\AF/6\n"
	"    7\\KG/  \\HH/  \\FI/  \\DH/  \\BG/7    \n"
	"       8\\IH/+ \\GI/+ \\EI/+ \\CH/8        \n"
	"          9\\HI/. \\FJ/. \\DI/9            \n"
	"            10\\GJ/  \\EJ/10               \n"
	"               11\\FK/11                   \n";


const command Commands::command_list[] = {
	{"end"       , command_quit      , "exits engine"                           },
	{"quit"      , command_quit      , "exits engine"                           },
	{"help"      , command_help      , "displays help"                          },
	{"display"   , command_display   , "displays board"                         },
	{"init"      , command_init      , "restarts engine"                        },
	{"new"       , command_new       , "starts a new game"                      },
	{"edit"      , command_edit      , "edits a position"                       },
	{"rotate"    , command_rotate    , "rotates board display"                  },
	{"fen"       , command_fen       , "read position from a fen string"        },
	{"white"     , command_white     , "white side to move"                     },
	{"black"     , command_black     , "black side to move"                     },
	{"moves"     , command_moves     , "list of pseudo legal moves"             },
	{""          , command_init      , "dummy"                                  }
};

bool Commands::rotate = false;

Commands::Commands()
{
}

void Commands::command_init()
{
	Hexbitboard::init();
	Attacks::init();
	Commands::rotate = false;
	MoveGen::reset_move_stack();
	Attacks::generate_attacks();
	cout << ENGINE_NAME;
}

void Commands::command_quit()
{
	cout << "Bye\n";
	exit(0);
}

void Commands::command_help()
{
	for (int i = 0; command_list[i].name != ""; ++i) {
		cout << command_list[i].name << "\t - " << command_list[i].description << endl;
	};
}

void Commands::command_display()
{
	if (rotate) {
		cout << recode_display(hexboard_display_rotate) << endl;
	}
	else {
		cout << recode_display(hexboard_display_normal) << endl;
	}

}

std::string Commands::recode_display(std::string hexboard_display)
{
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 11; ++j) {
			string piece = "";
			piece = piece + char(int('A') + i) + char(int('A') + j);
            size_t pos = hexboard_display.find(piece);
			if (pos != string::npos) {
				unsigned int base = HEX_A1 + i + (j * Hexbitboard::RANK_WIDTH);
				if (i > 8) base--;
				piece = Hexbitboard::get_men(base);
                hexboard_display = hexboard_display.replace(pos, 2, piece);
			}
		}
	}
	return hexboard_display;
}

void Commands::command_rotate()
{
	rotate = !rotate;
	command_display();
}

void Commands::command_fen()
{
	string xfen;
	cin >> xfen;
	if (Hexbitboard::setup_board(xfen))
	{
		if (!Attacks::position_is_ok()) { // check if opponent king is not checked
			cout << "position is illegal\n";
			Hexbitboard::restore_bitboards();
		}
		else {
			cout << "position is legal\n";
			MoveGen::reset_move_stack();
			Attacks::generate_attacks();
		}
	}
}

void Commands::command_new()
{
	Hexbitboard::new_game();
	MoveGen::white_to_move = true;
	MoveGen::reset_move_stack();
	Attacks::generate_attacks();
}

void Commands::command_edit()
{
	edit();
	if (!Attacks::position_is_ok()) {
		cout << "position is illegal\n";
		Hexbitboard::restore_bitboards();
	}
	else {
		cout << "position is legal\n";
		MoveGen::reset_move_stack();
		Attacks::generate_attacks();
	}
}

void Commands::edit()
{
	Hexbitboard::backup_bitboards();
	bool edit_white = true;
	string edit_line;
	string token;
	men piece;
	unsigned int base;
	char file;
	cout << "type '.' to exit, 'c' to change color\n";
	std::getline(cin, edit_line);
	for (;;) {
		if (edit_white) {
			cout << "edit (white): ";
		}
		else {
			cout << "edit (black): ";
		}
		std::getline(cin, edit_line);
		std::istringstream iss(edit_line);
		while( iss >> token ) {
			if (token == ".") {
				Hexbitboard::set_white_black();
				return;
			}
			else if (token == "c") {
				edit_white = !edit_white;
			}
			else if (token == "#") {
				Hexbitboard::clean_bitboards();
			}
			else {
				std::transform(token.begin(), token.end(),token.begin(), toupper);
				switch (token[0]) {
				case 'K':
					piece = edit_white ? WHITE_KING : BLACK_KING;
					break;
				case 'N':
					piece = edit_white ? WHITE_KNIGHT : BLACK_KNIGHT;
					break;
				default:
					piece = EMPTY;
					break;
				}
				if (piece == EMPTY) {
					continue;
				}
				if (token.length() > 2) {
					file = token[1];
					if ((file < 'A') || (file > 'L') || file == 'J') {
						continue;
					}
					else {
						base = (int) file - int('A') + HEX_A1;
					}
				}
				else {
					continue;
				}
				string num = token.substr(2);
				int rank = 0;
				std::istringstream intstream(num);
				intstream >> rank;
				if (rank <= 0) {
					continue;
				}
				else {
					switch (file) {
					case 'A': case 'L':
						if (rank > 6) { rank = 0; };
						break;
					case 'B': case 'K':
						if (rank > 7) { rank = 0; };
						break;
					case 'C': case 'I':
						if (rank > 8) { rank = 0; };
						break;
					case 'D': case 'H':
						if (rank > 9) { rank = 0; };
						break;
					case 'E': case 'G':
						if (rank > 10) { rank = 0; };
						break;
					case 'F':
						if (rank > 11) { rank = 0; };
						break;
					}
				}
				if (rank != 0) {
					if (file > 'I') {
						base--;
					}
					base = base + (rank-1) * Hexbitboard::RANK_WIDTH;
					Hexbitboard::set_piece(piece, base);
				}
			}
		}
	}
}

void Commands::command_white()
{
	MoveGen::white_to_move = true;
	if (Attacks::position_is_ok()) {
		cout << "position is legal\n";
		MoveGen::reset_move_stack();
		Attacks::generate_attacks();
	}
	else {
		cout << "position is illegal\n";
		MoveGen::white_to_move = false;
	}
}

void Commands::command_black()
{
	MoveGen::white_to_move = false;
	if (Attacks::position_is_ok()) {
		cout << "position is legal\n";
		MoveGen::reset_move_stack();
		Attacks::generate_attacks();
	}
	else {
		cout << "position is illegal\n";
		MoveGen::white_to_move = true;
	}
}

void Commands::command_moves()
{
	cout << MoveGen::get_legal_moves();
}

void Commands::read_commands()
{
	string command_line;
	void (*command)();
	while (true) {
		cout << "glaucus:";
		if (MoveGen::white_to_move) {
			cout << " (white)>";
		}
		else {
			cout << " (black)>";
		};
		cout.flush();
		cin >> command_line;
        command = nullptr;
		int i = 0;
		do {
			if (command_list[i].name == command_line) {
				command = command_list[i].function;
				break;
			}
			i++;
		} while (command_list[i].name != "");
		if (command) {
			command();
		}
		else {
			cout << "unrecognized command: " << command_line << endl;
		}
	}
}
