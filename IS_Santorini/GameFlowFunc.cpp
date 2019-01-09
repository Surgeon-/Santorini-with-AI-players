
#include "pch.h"

#include "GameFlowFunc.hpp"

#include <optional>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <stdexcept>
#include <regex>
#include <deque>

namespace etf_santorini_bj150087d {

    namespace flow {

        namespace {
        
            UINT _current_player;

            GameMove _last_move;
            std::optional<GameMove> _next_move;

            GameState _game_state, _last_state;
        
            UINT _game_phase;

            std::deque<std::string> _output_queue;

        }

        // Current player:

        UINT GetCurrentPlayer() {
        
            return _current_player;

        }

        void SetCurrentPlayer(UINT ind) {
        
            _current_player = ind;

        }

        // Next move:

        bool NextMoveReady() {
        
            return (_next_move.has_value());

        }

        GameMove PopNextMove() {
        
            GameMove rv = _next_move.value();

            _next_move.reset();

            return rv;

        }

        void PushNextMove(const GameMove & move) {
        
            _last_move = move;

            _next_move.emplace(move);

        }

        GameMove GetLastMove() {

            return _last_move;

        }

        // Game phase:

        UINT GetCurrentPhase() {
        
            return _game_phase;

        }

        void SetCurrentPhase(UINT phase) {
        
            _game_phase = phase;

        }

        // Game state:

        void SetCurrentState(const GameState & state) {
        
            _last_state = _game_state;

            _game_state = state;

        }

        GameState GetLastState() {
        
            return _last_state;
        
        }

        GameState GetCurrentState() {

            return _game_state;

        }

        void SetPawnPosition(UINT player_index, UINT pawn_index, Char2d pos) {
        
            _game_state.set_pos(player_index, pawn_index, pos);

        }

        // Reset whole game:

        void CompleteReset() {
        
            SetCurrentPlayer(0);

            if (NextMoveReady()) PopNextMove();

            SetCurrentPhase( PHASE_PLAYER_SETUP );

            SetCurrentState( GameState{} );

            ResetOutputQueue();

        }

        // Notify:

        void NotifyPlace(UINT player_index, const char * name, Char2d pos, UINT pawn_index) {

            char row = 'A' + pos.y;
            char col = '1' + pos.x;

            printf("Player %d (%7s): Placed their pawn on %c%c.\n", int(player_index + 1), name, row, col);

            if (pawn_index == 0) {
                _output_queue.emplace_back();
                _output_queue.back() += row;
                _output_queue.back() += col;
                _output_queue.back() += " ";
            }
            else {
                _output_queue.back() += row;
                _output_queue.back() += col;
            }

        }

        void NotifyMove(UINT player_index, const char * name, const GameMove & move, long time) {

            char move_buffer[10];

            char r1, r2, r3; // Rows (A-Z)
            char c1, c2, c3; // Columns (1-5)

            GameState curr = GetCurrentState();

            r1 = 'A' + curr.get_pos(player_index, move.pawn_index).y;
            c1 = '1' + curr.get_pos(player_index, move.pawn_index).x;

            r2 = 'A' + move.pos_coord.y;
            c2 = '1' + move.pos_coord.x;

            r3 = 'A' + move.bld_coord.y;
            c3 = '1' + move.bld_coord.x;

            snprintf(move_buffer, sizeof(move_buffer), "%c%c %c%c %c%c", r1, c1, r2, c2, r3, c3);

            printf("Player %d (%7s): Decided on a move (%s) in %5dms.\n", int(player_index + 1), name, move_buffer, int(time));

            _output_queue.push_back(move_buffer);

        }

        // File I/O:

        namespace {
        
            void SetupFromString(UINT player_index, UINT pawn_index, const std::string & str, UINT line_num) {
            
                char row = str[0] - 'A';
                char col = str[1] - '1';

                Char2d pos{col, row};

                GameState curr = GetCurrentState();

                for (UINT player = 0; player < 2; player += 1)
                    for (UINT pawn = 0; pawn < 2; pawn += 1) {
                        if (curr.get_pos(player, pawn) == pos)
                            throw std::runtime_error("SetupFromString(...) - Invalid argument (place already taken). at line " + std::to_string(line_num) + ".");
                    }

                SetPawnPosition(player_index, pawn_index, pos);
                NotifyPlace(player_index, "File_IN", pos, pawn_index);

            }

            void MoveFromString(UINT player_index, const std::string & s1, const std::string & s2, const std::string & s3, UINT line_num) {
            
                UINT pawn_index = 2;

                char row;
                char col;

                GameState curr = GetCurrentState();

                row = s1[0] - 'A'; // Initial pawn position
                col = s1[1] - '1';

                for (UINT pawn = 0; pawn < 2; pawn += 1) {
                    if (curr.get_pos(player_index, pawn) == Char2d{col, row}) {
                        pawn_index = pawn;
                        break;
                    }
                }

                if (pawn_index == 2)
                    throw std::runtime_error("MoveFromString(...) - Invalid initial pawn position [" + s1 + "] at line " + std::to_string(line_num) + ".");

                row = s2[0] - 'A'; // New pawn position
                col = s2[1] - '1';

                Char2d pos_coord{col, row};

                row = s3[0] - 'A'; // Build position
                col = s3[1] - '1';

                Char2d bld_coord{col, row};

                GameMove move{player_index, pawn_index, pos_coord, bld_coord};

                GameState newstate = curr.test_move(move);

                if (!newstate.is_valid())
                    throw std::runtime_error("MoveFromString(...) - Invalid move [" + s1 + " " + s2 + " " + s3 + "] at line " + std::to_string(line_num) + ".");

                SetCurrentState(newstate);
                NotifyMove(player_index, "File_IN", move, 0);

            }

        }

        void LoadStateFromFile(const char * path) {
        
            #define ST_SETUP_1 0
            #define ST_SETUP_2 1
            #define ST_MOVE    2

            const char * pattern_empty = R"_(^\s*(//.*)?)_";
            const char * pattern_setup = R"_(^\s*([A-E][1-5])\s\s*([A-E][1-5])\s*(//.*)?)_";
            const char * pattern_move  = R"_(^\s*([A-E][1-5])\s\s*([A-E][1-5])\s\s*([A-E][1-5])\s*(//.*)?)_";

            int  state    = ST_SETUP_1;
            UINT player   = GetCurrentPlayer();
            UINT line_num = 0;

            // Reset state:
            if (NextMoveReady()) PopNextMove();
            SetCurrentState( GameState{} );
            SetCurrentPhase( PHASE_PLAYER_SETUP );

            // Open file:
            std::ifstream file{path, std::ios::in};
            std::string   line;
            std::smatch   matches;

            if (!file.is_open()) {

                char buff[128];

                strerror_s(buff, sizeof(buff), errno);

                throw std::runtime_error( std::string("LoadStateFromFile(...) - Could not open file for reading: ") + buff );

            }

            while (std::getline(file, line)) {
            
                line_num += 1;

                if (std::regex_match(line, std::regex(pattern_empty))) continue;

                switch (state) {
                
                case ST_SETUP_1:
                    if (!std::regex_match(line, matches, std::regex(pattern_setup)) || matches.size() < 3) {
                        throw std::runtime_error("LoadStateFromFile(...) - Error in case ST_SETUP_1: Regex mismatch with line " + std::to_string(line_num) + " [" + line + "]");
                    }
                    SetupFromString(player, 0, matches[1], line_num);
                    SetupFromString(player, 1, matches[2], line_num);
                    player = ReversePlayerIndex(player);
                    SetCurrentPlayer(player);
                    state = ST_SETUP_2;
                    break;

                case ST_SETUP_2:
                    if (!std::regex_match(line, matches, std::regex(pattern_setup)) || matches.size() < 3) {
                        throw std::runtime_error("LoadStateFromFile(...) - Error in case ST_SETUP_2: Regex mismatch with line " + std::to_string(line_num) + " [" + line + "]");
                    }
                    SetupFromString(player, 0, matches[1], line_num);
                    SetupFromString(player, 1, matches[2], line_num);
                    player = ReversePlayerIndex(player);
                    SetCurrentPlayer(player);
                    state = ST_MOVE;
                    SetCurrentPhase(PHASE_PLAY);
                    break;

                case ST_MOVE:
                    if (!std::regex_match(line, matches, std::regex(pattern_move)) || matches.size() < 4) {
                        throw std::runtime_error("LoadStateFromFile(...) - Error in case ST_MOVE: Regex mismatch with line " + std::to_string(line_num) + " [" + line + "]");
                    }
                    MoveFromString(player, matches[1], matches[2], matches[3], line_num);
                    SetCurrentPlayer(player);
                    state = ST_MOVE;
                    break;

                default: 
                    NOP();
                    break;

                }

            }

            #undef ST_SETUP_1
            #undef ST_SETUP_2
            #undef ST_MOVE

        }

        void ResetOutputQueue() {
        
            _output_queue.clear();

        }

        bool OutputQueueHasData() {
        
            return (!_output_queue.empty());

        }

        void WriteOutputQueueToFile(const char * path) {
        
            std::ofstream file{path, std::ios::out | std::ios::trunc};

            if (!file.is_open()) {

                char buff[128];

                strerror_s(buff, sizeof(buff), errno);

                throw std::runtime_error( std::string("WriteOutputQueueToFile(...) - Could not open file for writing: ") + buff );

            }

            for (const auto & str : _output_queue) {
                
                file << str << "\n";

            }

        }

    }

}