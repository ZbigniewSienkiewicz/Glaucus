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

const bits128 Attacks::king_attacks[HEXES_NUMBER_MAX] = {
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{8604616704ULL,0ULL},
	{17211331584ULL,0ULL},
	{34422663168ULL,0ULL},
	{68845326336ULL,0ULL},
	{137556516864ULL,0ULL},
	{234962944ULL,0ULL},
	{137908895744ULL,0ULL},
	{275884867584ULL,0ULL},
	{551769735168ULL,0ULL},
	{1103539470336ULL,0ULL},
	{2202781876224ULL,0ULL},
	{17622255012864ULL,0ULL},
	{35248807091200ULL,0ULL},
	{70497614183424ULL,0ULL},
	{140995228366848ULL,0ULL},
	{281715746598912ULL,0ULL},
	{481204363264ULL,0ULL},
	{282437418975232ULL,0ULL},
	{565012209795072ULL,0ULL},
	{1130024419590144ULL,0ULL},
	{2260048837083136ULL,0ULL},
	{4511297284079616ULL,0ULL},
	{36090378266345472ULL,0ULL},
	{72189556922778624ULL,0ULL},
	{144379113847654400ULL,0ULL},
	{288758227695308800ULL,0ULL},
	{576953849034579968ULL,0ULL},
	{985506536046592ULL,0ULL},
	{578431834061406208ULL,0ULL},
	{1157145005660569600ULL,0ULL},
	{2314290011321139200ULL,0ULL},
	{4628580018347311104ULL,0ULL},
	{9239136837795053568ULL,0ULL},
	{126118394637320192ULL,4ULL},
	{270259988174209024ULL,8ULL},
	{540519980643385344ULL,16ULL},
	{1081039961286770688ULL,32ULL},
	{1009862105408471040ULL,64ULL},
	{2018317385823420416ULL,0ULL},
	{4036775440348610560ULL,64ULL},
	{8649730158023933952ULL,128ULL},
	{17299460316047867904ULL,256ULL},
	{16152167762293161984ULL,513ULL},
	{13839568251979300864ULL,1025ULL},
	{36055185298030592ULL,8206ULL},
	{90133569493532672ULL,16414ULL},
	{180275935080087552ULL,32828ULL},
	{360551870160175104ULL,65656ULL},
	{2162255621078908928ULL,131184ULL},
	{1443333655425449984ULL,224ULL},
	{3174756812075302912ULL,131520ULL},
	{5773052871847182336ULL,263104ULL},
	{11546105743694364672ULL,526208ULL},
	{4627453015169695744ULL,1052417ULL},
	{9236882835736887296ULL,2100736ULL},
	{54043195528445952ULL,24580ULL},
	{126109585659396096ULL,33611786ULL},
	{270233569828274176ULL,67231764ULL},
	{540467139656548352ULL,134463528ULL},
	{1080934279313096704ULL,268665072ULL},
	{4468274517793308672ULL,458912ULL},
	{8648037184458194944ULL,269353312ULL},
	{17296074368916389888ULL,538837632ULL},
	{16145404664123228160ULL,1077675265ULL},
	{13844065254536904704ULL,2151156225ULL},
	{9223372036854775808ULL,3146753ULL},
	{0ULL,0ULL},
	{18014398509481984ULL,100679694ULL},
	{36028797018963968ULL,137673875486ULL},
	{72057594037927936ULL,275381305404ULL},
	{144115188075855872ULL,550226067576ULL},
	{1441151880758558720ULL,939852272ULL},
	{2305843009213693952ULL,551635583936ULL},
	{4611686018427387904ULL,1103539472256ULL},
	{9223372036854775808ULL,2202783977216ULL},
	{0ULL,3222277633ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,412384026628ULL},
	{0ULL,563912193990664ULL},
	{0ULL,1126862986395664ULL},
	{0ULL,1924817453216ULL},
	{0ULL,1129749675901184ULL},
	{0ULL,2255650792669696ULL},
	{0ULL,3299612296192ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,1689124973068288ULL},
	{0ULL,2307533921161609216ULL},
	{0ULL,3942026144186368ULL},
	{0ULL,2309223736618254336ULL},
	{0ULL,3378802991300608ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,2309784621027426304ULL},
	{0ULL,2308662020259446784ULL},
	{0ULL,2307535708438396928ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,3942024063483904ULL}
};
const bits128 Attacks::knight_attacks[HEXES_NUMBER_MAX] = {
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{52810934648832ULL,0ULL},
	{105626164264960ULL,0ULL},
	{211252330627072ULL,0ULL},
	{422229783412736ULL,0ULL},
	{281784222875648ULL,0ULL},
	{343882596352ULL,0ULL},
	{282093988880384ULL,0ULL},
	{845525549072384ULL,0ULL},
	{1691188537065472ULL,0ULL},
	{3382372779163648ULL,0ULL},
	{6755949465305088ULL,0ULL},
	{108156794160812032ULL,0ULL},
	{216322384414646272ULL,0ULL},
	{432644773124259840ULL,0ULL},
	{864726596429317120ULL,0ULL},
	{577094088449460224ULL,0ULL},
	{704271557595136ULL,0ULL},
	{577728489227558912ULL,0ULL},
	{1731636324501323776ULL,0ULL},
	{3463554123910152192ULL,0ULL},
	{6927099451727282176ULL,0ULL},
	{13836184504945082368ULL,0ULL},
	{144185556828424192ULL,12ULL},
	{306385512166330368ULL,24ULL},
	{612779820425682944ULL,48ULL},
	{72638411124524032ULL,96ULL},
	{1297072427083335680ULL,64ULL},
	{1442348149955104768ULL,0ULL},
	{2596325220629921792ULL,64ULL},
	{4616330426478297088ULL,192ULL},
	{9809121463523999744ULL,384ULL},
	{1153484454828965888ULL,769ULL},
	{2306968909657931776ULL,1536ULL},
	{144115205259919360ULL,24592ULL},
	{288230410519839744ULL,49186ULL},
	{594475219549162496ULL,98372ULL},
	{1189513393348810752ULL,196616ULL},
	{73184052496052224ULL,131216ULL},
	{2449959314526527488ULL,160ULL},
	{4611758621729357824ULL,131360ULL},
	{9511747688262795264ULL,393728ULL},
	{576461027451338752ULL,787521ULL},
	{1152922054900580352ULL,1575040ULL},
	{2305844109799063552ULL,3145984ULL},
	{35192962023424ULL,33587216ULL},
	{70385926144000ULL,100728864ULL},
	{140771854385152ULL,201465922ULL},
	{1153211848703606784ULL,402669700ULL},
	{2306986922238541824ULL,268730376ULL},
	{2288101330255872ULL,327952ULL},
	{148691562434396160ULL,269025792ULL},
	{297523724918194176ULL,806356000ULL},
	{563502930460672ULL,1612843072ULL},
	{1127001565954048ULL,3221487744ULL},
	{2253998836940800ULL,2148008192ULL},
	{72075186223972352ULL,67141632ULL},
	{144150376742912000ULL,137573236736ULL},
	{288300757780791296ULL,412585426944ULL},
	{594624710164086784ULL,824667545728ULL},
	{2342733874888441856ULL,550359810304ULL},
	{4686031524364025856ULL,671645696ULL},
	{9372414686290509824ULL,550964822032ULL},
	{586034200046469120ULL,1651417088033ULL},
	{1154054001583456256ULL,3298803449856ULL},
	{2308099207073890304ULL,2199560388608ULL},
	{4616189618054758400ULL,1074266112ULL},
	{0ULL,0ULL},
	{72066390130950144ULL,275012124688ULL},
	{144141576354922496ULL,563499977670688ULL},
	{306297551219326976ULL,1688850397397058ULL},
	{1765516607045500928ULL,1127136891502852ULL},
	{4685643568558112768ULL,1375530385928ULL},
	{10091440865030438912ULL,1128375955522576ULL},
	{1159676904047902720ULL,3377699854813249ULL},
	{2319353808095805440ULL,2252349837934720ULL},
	{4620693217682128896ULL,1100048498944ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{54043195528445952ULL,1126449662722064ULL},
	{108086391056891904ULL,2305844109262323746ULL},
	{216172782113783808ULL,2308094810101653700ULL},
	{3891110078048108544ULL,2817086230381064ULL},
	{6917529027641081856ULL,2306405959236355168ULL},
	{13835058055282163712ULL,2305843284225951872ULL},
	{9223372036854775808ULL,1126449662918913ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,2252899593814028ULL},
	{0ULL,2200097398808ULL},
	{0ULL,2182103472ULL},
	{0ULL,137508356864ULL},
	{0ULL,563225100027392ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,2199828611072ULL},
	{0ULL,2181922816ULL},
	{0ULL,137843179520ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,0ULL},
	{0ULL,1811939328ULL}
};

bits128 Attacks::opponent_attacks;
bits128 Attacks::own_attacks;


Attacks::Attacks()
{
}

void Attacks::init()
{
    generate_own_attacks();
    generate_opponent_attacks();
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
