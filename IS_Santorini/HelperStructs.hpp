#pragma once

#include "Utility.hpp"

#include <string>

namespace etf_santorini_bj150087d {

    struct Char2d {
    
        char x, y;

        Char2d(char x = 0, char y = 0): x(x), y(y) { }

        bool operator==(const Char2d& other) const { 
            return (x == other.x && y == other.y);
        };

        bool operator!=(const Char2d& other) const { 
            return (!(x == other.x && y == other.y));
        };

    };

    struct GameMove;

    class GameState {
    
    public:

        GameState();

        // Copy:
        GameState(const GameState& other) = default;
        GameState& operator=(const GameState & other) = default;

        // Move:
        GameState(GameState&& other) = default;
        GameState& operator=(GameState && other) = default;

        // Methods:
        bool  is_valid() const;
        char& at(UINT x, UINT y);
        char  at(UINT x, UINT y) const;

        Char2d get_pos(UINT player_index, UINT pawn_index) const;
        void   set_pos(UINT player_index, UINT pawn_index, Char2d pos);

        GameState test_move(const GameMove& move) const;
        void apply_move(const GameMove& move);

        // bool test_victory(UINT curr_player, UINT & victor); // Curr_player = whose turn is it (globally)

    private:

        char   board[5][5];
        Char2d position[2][2]; // [Player][Pawn]

        bool valid;

    };

    struct GameMove {

        UINT player_index;
        UINT pawn_index;
        
        Char2d pos_coord, bld_coord;

        GameMove(UINT player_index = 0, UINT pawn_index = 0, Char2d pos_coord = Char2d{}, Char2d bld_coord = Char2d{})
            : player_index(player_index)
            , pawn_index(pawn_index)
            , pos_coord(pos_coord)
            , bld_coord(bld_coord)
            { }

        // Copy:
        GameMove(const GameMove& other) = default;
        GameMove& operator=(const GameMove & other) = default;

        // Move:
        GameMove(GameMove&& other) = default;
        GameMove& operator=(GameMove && other) = default;

        // Methods:
        //std::string to_string() const;

    };

}