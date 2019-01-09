#pragma once

#include "pch.h"

#include "Utility.hpp"
#include "HelperStructs.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace etf_santorini_bj150087d {

    namespace {
    
        bool IsAdjecent(Char2d pos1, Char2d pos2) {
        
            if (std::abs(pos2.x - pos1.x) > 1) return false;

            if (std::abs(pos2.y - pos1.y) > 1) return false;

            return true;

        }

    }

    GameState::GameState() {
    
        UINT i, t;

        for (i = 0; i < 5u; i += 1)
            for (t = 0; t < 5u; t += 1)
                board[i][t] = char(0);

        for (i = 0; i < 2; i += 1)
            for (t = 0; t < 2; t += 1)
                position[i][t] = Char2d(char(-1), char(-1));

        valid = true;

    }

    char& GameState::at(UINT x, UINT y) {
    
        if (x > 4 || y > 4) throw std::logic_error("GameState::at(...) - Index out of bounds!");

        return board[x][y];

    }

    char  GameState::at(UINT x, UINT y) const {

        if (x > 4 || y > 4) {
            
            throw std::logic_error("GameState::at(...) - Index out of bounds!");

            }

        return board[x][y];

    }

    Char2d GameState::get_pos(UINT player_index, UINT pawn_index) const {
    
        if (player_index > 1 || pawn_index > 1) throw std::logic_error("GameState::get_pos(...) - Index out of bounds!");

        return position[player_index][pawn_index];

    }

    void   GameState::set_pos(UINT player_index, UINT pawn_index, Char2d pos) {
    
        if (player_index > 1 || pawn_index > 1) throw std::logic_error("GameState::set_pos(...) - Index out of bounds!");

        position[player_index][pawn_index] = pos;

    }

    bool  GameState::is_valid() const {
    
        return valid;

    }

    GameState GameState::test_move(const GameMove& move) const {
    
        GameState rv{*this};

        #define RETURN_INVALID() do { rv.valid = false; return rv; } while (0)
        #define PLAYER_IND move.player_index
        #define PAWN_IND   move.pawn_index

        if (!valid) RETURN_INVALID();

        // MOVE ///////////////////////////////////////////////////////////////

        // Zero move
        if (get_pos(PLAYER_IND, PAWN_IND) == move.pos_coord)
            RETURN_INVALID();

        // Nonadjecent move
        if (!IsAdjecent(get_pos(PLAYER_IND, PAWN_IND), move.pos_coord))
            RETURN_INVALID();

        // Edge of board
        if (move.pos_coord.x < 0 || move.pos_coord.y < 0 ||
            move.pos_coord.x > 4 || move.pos_coord.y > 4 )
            RETURN_INVALID();

        // Dome
        if (at(UINT(move.pos_coord.x), UINT(move.pos_coord.y)) == 4)
            RETURN_INVALID();

        // Another pawn
        for (UINT i = 0; i < 2; i += 1)
            for (UINT t = 0; t < 2; t += 1)
                if (position[i][t] == move.pos_coord)
                    RETURN_INVALID();

        // Height difference > 1
        if (at(UINT(move.pos_coord.x), UINT(move.pos_coord.y)) > 
            at(UINT(get_pos(PLAYER_IND, PAWN_IND).x), UINT(get_pos(PLAYER_IND, PAWN_IND).y)) + 1 )
            RETURN_INVALID();

        // Move is valid
        rv.set_pos(PLAYER_IND, PAWN_IND, move.pos_coord); // From now on we need to check stuff
                                                          // against rv and not *this

        // BUILD //////////////////////////////////////////////////////////////

        // Edge of board
        if (move.bld_coord.x < 0 || move.bld_coord.y < 0 ||
            move.bld_coord.x > 4 || move.bld_coord.y > 4 )
            RETURN_INVALID();

        // Can't build over pawns
        for (UINT i = 0; i < 2; i += 1)
            for (UINT t = 0; t < 2; t += 1)
                if (rv.position[i][t] == move.bld_coord)
                    RETURN_INVALID();

        // Can't build over domes
        if (rv.at(UINT(move.bld_coord.x), UINT(move.bld_coord.y)) >= 4)
            RETURN_INVALID();

        // Must build next to moved worker
        if (!IsAdjecent( move.pos_coord, move.bld_coord))
            RETURN_INVALID();

        // Build is valid
        rv.at(UINT(move.bld_coord.x), UINT(move.bld_coord.y)) += 1;  

        // FINALIZE ///////////////////////////////////////////////////////////

        return rv;

        #undef RETURN_INVALID
        #undef PLAYER_IND
        #undef PAWN_IND

    }

    void GameState::apply_move(const GameMove& move) {
    
        *this = test_move(move);
    
    }

    /*bool GameState::test_victory(UINT player_index, UINT & victor) {

        // Standing on height 3:
        Char2d pos0 = position[player_index][0];
        Char2d pos1 = position[player_index][1];

        if (board[pos0.x][pos0.y] == 3 || board[pos1.x][pos1.y] == 3) return true;

        // Opponent can't move:
        UINT opp = ReversePlayerIndex(player_index);

        for (UINT pawn = 0; pawn < 2; pawn += 1)
            for (char xoff = -1; xoff <= 1; xoff += 1)
                for (char yoff = -1; yoff <= 1; yoff += 1) {
        
                    if (xoff == 0 and yoff == 0) continue;

                    Char2d temp{ position[opp][pawn] };
                    temp.x += xoff;
                    temp.y += yoff;

                }

    }*/

    /*std::string GameMove::to_string() const {
    
        // X = Letters, Y = Numbers

        return std::string("PLAYER ") + char('1' + player_index) + ": " +
               "Move the " + ((pawn_index == 0) ? "1st " : "2nd ") + "pawn to [" +
               char('A' + pos_coord.x) + "," + char('1' + pos_coord.y) + "] and build on [" +
               char('A' + bld_coord.x) + "," + char('1' + bld_coord.y) + "].";

    }*/

}