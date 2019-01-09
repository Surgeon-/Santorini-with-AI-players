
#include "pch.h"

#include "Utility.hpp"
#include "HumanPlayer.hpp"
#include "QAO-Module.hpp"
#include "ObjectTID.hpp"
#include "Gr-Module.hpp"
#include "IOMgr.hpp"
#include "HelperStructs.hpp"
#include "GameFlowFunc.hpp"
#include "GlobalStore.hpp"

#include <chrono>
#include <iostream>
#include <cmath>

using namespace etf_santorini_bj150087d::gr;

namespace etf_santorini_bj150087d {

    namespace {

        bool IsAdjecent(Char2d pos1, Char2d pos2) {

            if (std::abs(pos2.x - pos1.x) > 1) return false;

            if (std::abs(pos2.y - pos1.y) > 1) return false;

            return true;

        }

    }

    namespace so {
    
        HumanPlayer::HumanPlayer(UINT player_index) 
            : qao::Base(TID::HumanPlayer, -5, "HumanPlayer") 
            , player_index(player_index) {
        
            state = State::Idle;
            skipped_last = true;

        }

        HumanPlayer::~HumanPlayer() {
        
        }

        void HumanPlayer::event_update() {
        
            if (flow::GetCurrentPlayer() != player_index || flow::NextMoveReady()) {
                skipped_last = true;   
                return;
            } else {
                if (skipped_last) turn_begin_time = std::chrono::steady_clock::now();
                skipped_last = false;
            }

            if (flow::GetCurrentPhase() == flow::PHASE_PLAYER_SETUP) {

                exec_setup();

                turn_begin_time = std::chrono::steady_clock::now();

            }
            else if (flow::GetCurrentPhase() == flow::PHASE_PLAY) {

                switch (state) {
            
                case State::Idle:
                    exec_idle();
                    break;

                case State::ClickedPawn:
                    exec_clpawn();
                    break;

                case State::ClickedMove:
                    exec_clmove();
                    break;

                default:
                    NOP();
                    break;

                }

            }
            else
                assert(0);

        }

        void HumanPlayer::event_draw_1() {
        
            if (flow::GetCurrentPlayer() != player_index) return;

            if (flow::GetCurrentPhase() == flow::PHASE_PLAYER_SETUP) {

                draw_setup();

            }
            else if (flow::GetCurrentPhase() == flow::PHASE_PLAY) {

                switch (state) {

                case State::Idle:
                    draw_idle();
                    break;

                case State::ClickedPawn:
                    draw_clpawn();
                    break;

                case State::ClickedMove:
                    draw_clmove();
                    break;

                default:
                    NOP();
                    break;

                }

            }
            else
                assert(0);

        }

        void HumanPlayer::event_draw_g() {
        
            if (flow::GetCurrentPlayer() != player_index || flow::NextMoveReady()) return;

            char buff[32];

            snprintf(buff, sizeof(buff), "Player %d (%s): ", int(player_index + 1), "Human");

            sf::String str = buff;

            if (flow::GetCurrentPhase() == flow::PHASE_PLAYER_SETUP) {

                str += "Place your pawns on the board.";

            }
            else if (flow::GetCurrentPhase() == flow::PHASE_PLAY) {
                
                switch (state) {

                case State::Idle:
                    str += "Choose one of your pawns to move.";
                    break;

                case State::ClickedPawn:
                    str += "Choose destination (Right-click to cancel).";
                    break;

                case State::ClickedMove:
                    str += "Choose a place to build (Right-click to cancel).";
                    break;

                default:
                    NOP();
                    break;

                }

            }
            else 
                assert(0);

            draw_set_alpha(255);
            draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);
            draw_set_color((player_index == 0) ? COL_ORANGE : COL_PURPLE);
            draw_text(12.0, 16.0, str, 24);

        }

        // Private:

        void HumanPlayer::exec_setup() {
        
            if (IOMgr::mb_check_pressed(sf::Mouse::Left)) {

                auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

                // Assume the board is drawn from [0,0] on the view

                int grid_x = int(mpos.x) / 128 - 1;
                int grid_y = int(mpos.y) / 128 - 1;

                if (grid_x < 0 || grid_x >= 5 || grid_y < 0 || grid_y >= 5) return;

                GameState curr = flow::GetCurrentState();

                Char2d newpos = Char2d{ char(grid_x), char(grid_y) };

                for (UINT j = 0; j < 2; j += 1)
                    for (UINT k = 0; k < 2; k += 1)
                        if (curr.get_pos(j, k) == newpos) return;

                for (UINT j = 0; j < 2; j += 1)
                    if (curr.get_pos(player_index, j) == Char2d{-1, -1}) {
                        flow::SetPawnPosition(player_index, j, newpos);
                        flow::NotifyPlace(player_index, "Human", newpos, j);
                        return;
                    }

            }

        }

        void HumanPlayer::exec_idle() {

            if (IOMgr::mb_check_pressed(sf::Mouse::Left)) {
            
                auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

                // Assume the board is drawn from [0,0] on the view

                int grid_x = int(mpos.x) / 128 - 1;
                int grid_y = int(mpos.y) / 128 - 1;

                const GameState gs = flow::GetCurrentState();

                if (gs.get_pos(player_index, 0u) == Char2d(char(grid_x), char(grid_y))) {
                
                    pawn_index = 0u;
                    pwn_coord = Char2d(char(grid_x), char(grid_y));
                    state = State::ClickedPawn;

                }
                else if (gs.get_pos(player_index, 1u) == Char2d(char(grid_x), char(grid_y))) {
                
                    pawn_index = 1u;
                    pwn_coord = Char2d(char(grid_x), char(grid_y));
                    state = State::ClickedPawn;

                }

            }

        }

        void HumanPlayer::exec_clpawn() {
        
            if (IOMgr::mb_check_pressed(sf::Mouse::Right)) {
                state = State::Idle;
                return;
            }

            if (IOMgr::mb_check_pressed(sf::Mouse::Left)) {

                auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

                // Assume the board is drawn from [0,0] on the view

                int grid_x = int(mpos.x) / 128 - 1;
                int grid_y = int(mpos.y) / 128 - 1;
                
                if (grid_x < 0 || grid_x > 4) return;
                if (grid_y < 0 || grid_y > 4) return;

                const GameState gs = flow::GetCurrentState();

                if ( pwn_coord == Char2d{char(grid_x), char(grid_y)} ) return;
                if ( !IsAdjecent(pwn_coord, Char2d{char(grid_x), char(grid_y)}) ) return;
                if ( gs.at(UINT(grid_x), UINT(grid_y)) > gs.at(UINT(pwn_coord.x), UINT(pwn_coord.y)) + 1 ) return;

                for (UINT player = 0; player < 2; player += 1)
                    for (UINT pawn = 0; pawn < 2; pawn += 1)
                        if ( gs.get_pos(player, pawn) == Char2d{char(grid_x), char(grid_y)}) return;

                mov_coord = Char2d{char(grid_x), char(grid_y)};
                state = State::ClickedMove;

            }

        }

        void HumanPlayer::exec_clmove() {
        
            if (IOMgr::mb_check_pressed(sf::Mouse::Right)) {
                state = State::Idle;
                return;
            }

            if (IOMgr::mb_check_pressed(sf::Mouse::Left)) {

                auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

                // Assume the board is drawn from [0,0] on the view

                int grid_x = int(mpos.x) / 128 - 1;
                int grid_y = int(mpos.y) / 128 - 1;

                if (grid_x < 0 || grid_x > 4) return;
                if (grid_y < 0 || grid_y > 4) return;

                const GameState gs = flow::GetCurrentState();

                if ( pwn_coord == Char2d{char(grid_x), char(grid_y)} ) goto L_OK;
                if ( mov_coord == Char2d{char(grid_x), char(grid_y)} ) return;
                if ( !IsAdjecent(mov_coord, Char2d{char(grid_x), char(grid_y)}) ) return;
                if ( gs.at(UINT(grid_x), UINT(grid_y)) == 4 ) return;

                for (UINT player = 0; player < 2; player += 1)
                    for (UINT pawn = 0; pawn < 2; pawn += 1)
                        if ( gs.get_pos(player, pawn) == Char2d{char(grid_x), char(grid_y)}) return;

                L_OK:

                bld_coord = Char2d{char(grid_x), char(grid_y)};

                GameMove move{player_index, pawn_index, mov_coord, bld_coord};
                flow::PushNextMove(move);

                long time_elapsed = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - turn_begin_time).count();
                flow::NotifyMove(player_index, "Human", move, time_elapsed);

                state = State::Idle;

            }

        }

        namespace {

            void DoHighlightCell(int x, int y) {

                double dx = double(x + 1) * 128.0;
                double dy = double(y + 1) * 128.0;

                draw_set_color(sf::Color::White);
                draw_set_alpha(128);

                draw_rectangle(dx, dy, 128.0, 128.0);

            }

        }

        void HumanPlayer::draw_setup() {

            auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

            // Assume the board is drawn from [0,0] on the view

            int grid_x = int(mpos.x) / 128 - 1;
            int grid_y = int(mpos.y) / 128 - 1;

            if (grid_x < 0 || grid_x >= 5 || grid_y < 0 || grid_y >= 5) return;

            GameState curr = flow::GetCurrentState();
                
            for (UINT player = 0; player < 2; player += 1)
                for (UINT pawn = 0; pawn < 2; pawn += 1)
                    if (curr.get_pos(player, pawn) == Char2d{char(grid_x), char(grid_y)}) return;
                    
            DoHighlightCell(grid_x, grid_y);

        }

        void HumanPlayer::draw_idle() {
        
            auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

            // Assume the board is drawn from [0,0] on the view

            int grid_x = int(mpos.x) / 128 - 1;
            int grid_y = int(mpos.y) / 128 - 1;

            const GameState gs = flow::GetCurrentState();

            if (gs.get_pos(player_index, 0u) == Char2d(char(grid_x), char(grid_y))) {

                DoHighlightCell(grid_x, grid_y);

            }
            else if (gs.get_pos(player_index, 1u) == Char2d(char(grid_x), char(grid_y))) {

                DoHighlightCell(grid_x, grid_y);

            }

        }

        void HumanPlayer::draw_clpawn() {
        
            DoHighlightCell(pwn_coord.x, pwn_coord.y);

            ///////////////////////////////////////////////////////////////////

            auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

            // Assume the board is drawn from [0,0] on the view

            int grid_x = int(mpos.x) / 128 - 1;
            int grid_y = int(mpos.y) / 128 - 1;

            if (grid_x < 0 || grid_x > 4) return;
            if (grid_y < 0 || grid_y > 4) return;

            const GameState gs = flow::GetCurrentState();

            if ( pwn_coord == Char2d{char(grid_x), char(grid_y)} ) return;
            if ( !IsAdjecent(pwn_coord, Char2d{char(grid_x), char(grid_y)}) ) return;
            if ( gs.at(UINT(grid_x), UINT(grid_y)) > gs.at(UINT(pwn_coord.x), UINT(pwn_coord.y)) + 1 ) return;

            for (UINT player = 0; player < 2; player += 1)
                for (UINT pawn = 0; pawn < 2; pawn += 1)
                    if ( gs.get_pos(player, pawn) == Char2d{char(grid_x), char(grid_y)}) return;

            DoHighlightCell(grid_x, grid_y);

            draw_set_alpha(255);
            draw_set_color(sf::Color::Red);
            draw_line( (double(pwn_coord.x) + 1.5) * 128.0
                     , (double(pwn_coord.y) + 1.5) * 128.0
                     , (double(grid_x) + 1.5) * 128.0
                     , (double(grid_y) + 1.5) * 128.0
                     , 2.0
                     ) ;
            draw_set_color(sf::Color::White);
            double angle = 180 * std::atan2(grid_y - pwn_coord.y, grid_x - pwn_coord.x) / M_PI;
            draw_sprite( (double(grid_x) + 1.5) * 128.0
                       , (double(grid_y) + 1.5) * 128.0
                       , sref("arrow")
                       , 0u
                       , Transform2d{Percent(0.5), Percent(0.5), 1.0, 1.0, angle + 90.0}
                       ) ;

        }

        void HumanPlayer::draw_clmove() {
        
            DoHighlightCell(pwn_coord.x, pwn_coord.y);
            DoHighlightCell(mov_coord.x, mov_coord.y);
            draw_set_alpha(255);
            draw_set_color(sf::Color::Red);
            draw_line( (double(pwn_coord.x) + 1.5) * 128.0
                     , (double(pwn_coord.y) + 1.5) * 128.0
                     , (double(mov_coord.x) + 1.5) * 128.0
                     , (double(mov_coord.y) + 1.5) * 128.0
                     , 2.0
                     ) ;
            double angle1 = 180 * std::atan2(mov_coord.y - pwn_coord.y, mov_coord.x - pwn_coord.x) / M_PI;
            draw_sprite( (double(mov_coord.x) + 1.5) * 128.0
                       , (double(mov_coord.y) + 1.5) * 128.0
                       , sref("arrow")
                       , 0u
                       , Transform2d{Percent(0.5), Percent(0.5), 1.0, 1.0, angle1 + 90.0}
                       ) ;

            ///////////////////////////////////////////////////////////////////

            auto mpos = IOMgr::mouse_pos(IOMgr::VIEW_DEFAULT);

            // Assume the board is drawn from [0,0] on the view

            int grid_x = int(mpos.x) / 128 - 1;
            int grid_y = int(mpos.y) / 128 - 1;

            if (grid_x < 0 || grid_x > 4) return;
            if (grid_y < 0 || grid_y > 4) return;

            const GameState gs = flow::GetCurrentState();

            if ( pwn_coord == Char2d{char(grid_x), char(grid_y)} ) goto L_OK;
            if ( mov_coord == Char2d{char(grid_x), char(grid_y)} ) return;
            if ( !IsAdjecent(mov_coord, Char2d{char(grid_x), char(grid_y)}) ) return;
            if ( gs.at(UINT(grid_x), UINT(grid_y)) == 4 ) return;

            for (UINT player = 0; player < 2; player += 1)
                for (UINT pawn = 0; pawn < 2; pawn += 1)
                    if ( gs.get_pos(player, pawn) == Char2d{char(grid_x), char(grid_y)}) return;

            L_OK:

            DoHighlightCell(grid_x, grid_y);

            draw_set_alpha(255);
            draw_set_color(sf::Color::Red);
            draw_line( (double(mov_coord.x) + 1.5) * 128.0
                     , (double(mov_coord.y) + 1.5) * 128.0
                     , (double(grid_x) + 1.5) * 128.0
                     , (double(grid_y) + 1.5) * 128.0
                     , 2.0
                     ) ;
            double angle2 = 180 * std::atan2(grid_y - mov_coord.y, grid_x - mov_coord.x) / M_PI;
            draw_sprite( (double(grid_x) + 1.5) * 128.0
                       , (double(grid_y) + 1.5) * 128.0
                       , sref("arrow")
                       , 0u
                       , Transform2d{Percent(0.5), Percent(0.5), 1.0, 1.0, angle2 + 90.0}
                       ) ;

        }
                
    }

}