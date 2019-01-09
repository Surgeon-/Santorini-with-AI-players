
#include "pch.h"

#include "GameController.hpp"
#include "QAO-Module.hpp"
#include "ObjectTID.hpp"
#include "Utility.hpp"
#include "GlobalStore.hpp"
#include "MinimaxTree.hpp"
#include "GameFlowFunc.hpp"
#include "Gr-Module.hpp"
#include "CellHighlight.hpp"
#include "Greeter.hpp"

#include <iostream>
#include <stdexcept>

extern void ClearConsole();

using namespace etf_santorini_bj150087d::gr;

namespace etf_santorini_bj150087d {

    #define _IA interpolation_amount
    #define CELL_SIZE 128.0

    namespace so {

        namespace {
        
            void DrawBoard(sf::RenderTarget & target, double sx, double sy) {

                draw_set_alpha(255);
                draw_set_color(sf::Color::White);
                //draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);

                draw_sprite(target, sx, sy, sref("board"), 0u);

                const GameState state = flow::GetCurrentState();

                for (int i = 0; i < 5; i += 1)
                    for (int t = 0; t < 5; t+= 1) {
                    
                        double dx = (i + 1) * CELL_SIZE + sx;
                        double dy = (t + 1) * CELL_SIZE + sy;

                        //draw_set_color(sf::Color::White);
                        //draw_rectangle_outline(dx, dy, 128.0, 128.0, 1.0);

                        int val = state.at(UINT(i), UINT(t));
                        
                        if (val >= 1 && val <= 4) {

                            draw_set_color(sf::Color::White);
                            draw_sprite(target, dx, dy, sref( std::string("building") + char('0' + val) ), 0);

                            for (int i = 0; i < val; i += 1) {

                                sf::Color tcol = (val == 1) ? (sf::Color::White)  :
                                                 (val == 2) ? (sf::Color::Yellow) :
                                                 (val == 3) ? (sf::Color::Red)    : (sf::Color::Blue);

                                draw_set_color(tcol);
                                draw_rectangle(target, dx + 4.0 + (i * 12.0), dy + 4.0, 8.0, 8.0);
                                draw_set_color(sf::Color::Black);
                                draw_rectangle_outline(target, dx + 4.0 + (i * 12.0), dy + 4.0, 8.0, 8.0, 2.0);

                            }

                        }

                        /*sf::String str = char((val < 4) ? ('0' + val) : ('D'));
                        draw_set_color(sf::Color::Green);
                        draw_text(dx + 4.0, dy + 4.0, str, 20u);*/

                    }

                ;

            }

            void DrawPawn(double sx, double sy, const GameState & sprev, const GameState & scurr, UINT player, UINT pawn, double interpolation_amount) {

                const double CXOFF = 8.0, CYOFF = 56.0;

                double x0 = sx + CELL_SIZE * (sprev.get_pos(player, pawn).x + 1) + CXOFF;
                double y0 = sy + CELL_SIZE * (sprev.get_pos(player, pawn).y + 1) + CYOFF;

                double x1 = sx + CELL_SIZE * (scurr.get_pos(player, pawn).x + 1) + CXOFF;
                double y1 = sy + CELL_SIZE * (scurr.get_pos(player, pawn).y + 1) + CYOFF;

                double dx = x0 + (x1 - x0) * _IA;
                double dy = y0 + (y1 - y0) * _IA;

                ///////////////////////////////////////////////////////////////

                /*draw_set_alpha(255);
                draw_set_color((player == 0) ? sf::Color::Blue : sf::Color::Red);
                draw_rectangle(dx, dy, 32.0, 64.0);
                draw_set_color(sf::Color::Yellow);
                draw_rectangle_outline(dx, dy, 32.0, 64.0, 1.0);*/

                draw_set_alpha(255);
                draw_set_color(sf::Color::White);
                draw_sprite(dx, dy, sref((player == 0) ? "pawn1" : "pawn2"), 0u);

                draw_set_color( (player == 0) ? COL_ORANGE : COL_PURPLE );
                draw_rectangle_outline(dx, dy, 56.0, 64.0, 1.0);

            }

        }

        GameController::GameController()
            : qao::Base(TID::GameController, 0, "GameController") {
        
            cooldown = 0;

            initial_render = false;
            fast_mode      = false;
            compact_mode   = false;
            sbs_mode       = false;

            if (!board_texture.create(UINT(CELL_SIZE) * 7u, UINT(CELL_SIZE) * 7u)) 
                throw std::runtime_error("so::GameController::GameController(...) - Couldn't create board_texture.");

        }

        GameController::~GameController() {
        
            // Write game history to the output file in the case of an exception being thrown:
            if (!GlobalSt.output_file_path.empty() && flow::OutputQueueHasData()) {

                flow::WriteOutputQueueToFile(GlobalSt.output_file_path.c_str());
                flow::ResetOutputQueue();

            }

        }

        void GameController::event_update() {

            // No players:
            if (!GlobalSt.player[0].exists() || !GlobalSt.player[1].exists()) {
            
                victor = NO_VICTOR;
                flow::CompleteReset();
                initial_render = false;
                fast_mode = false;
                return;

            }

            // Fast mode:
            if (IOMgr::kb_check_pressed(sf::Keyboard::F)) {
                
                fast_mode = !fast_mode;
                std::cout << "Fast mode is now " << (fast_mode ? "on." : "off.") << '\n';

            }

            // Compact mode:
            if (IOMgr::kb_check_pressed(sf::Keyboard::C)) {
            
                compact_mode = !compact_mode;

                UINT size = (compact_mode) ? 720u : 896u;

                IOMgr::get_window().setSize(sf::Vector2u(size, size));

                so::IOMgr::view_set_active(so::IOMgr::VIEW_GUI, true);
                so::IOMgr::view_set_center(so::IOMgr::VIEW_GUI, float(size / 2), float(size / 2));
                so::IOMgr::view_set_size(so::IOMgr::VIEW_GUI, float(size), float(size));
                so::IOMgr::view_set_viewport_r(so::IOMgr::VIEW_GUI, 0.f, 0.f, 1.f, 1.f);
                so::IOMgr::view_refresh_gui();

                std::cout << "Compact mode is now " << (compact_mode ? "on." : "off.") << '\n';

            }

            // Step-by-step mode:
            if (IOMgr::kb_check_pressed(sf::Keyboard::S)) {

                sbs_mode = !sbs_mode;

                std::cout << "Step-by-step mode is now " << (sbs_mode ? "on." : "off.") << '\n';

            }            

            GameState curr;
            UINT curpl = flow::GetCurrentPlayer();

            switch (flow::GetCurrentPhase()) {

            case flow::PHASE_PLAYER_SETUP:

                curr = flow::GetCurrentState();

                if (curr.get_pos(0, 0) != Char2d{-1, -1} &&
                    curr.get_pos(0, 1) != Char2d{-1, -1} && 
                    curr.get_pos(1, 0) != Char2d{-1, -1} &&
                    curr.get_pos(1, 1) != Char2d{-1, -1}) {
                
                    flow::SetCurrentPlayer( ReversePlayerIndex(curpl) );
                    flow::SetCurrentPhase( flow::PHASE_PLAY );
                    return;

                }

                if (curr.get_pos(curpl, 0) != Char2d{-1, -1} &&
                    curr.get_pos(curpl, 1) != Char2d{-1, -1}) {
                
                    flow::SetCurrentPlayer( ReversePlayerIndex(curpl) );
                    return;

                }

                break;

            case flow::PHASE_PLAY:

                if (cooldown > 1) { // Just reduce cooldown

                    cooldown = fast_mode ? 1 : (cooldown - 1);
                    return;

                }
                else if (cooldown == 1) { // Final cooldown reduction

                    if (victor == NO_VICTOR) {
                
                        if (!sbs_mode || IOMgr::kb_check_pressed(sf::Keyboard::N)) {

                            cooldown = 0;
                            flow::SetCurrentPlayer( flow::GetCurrentPlayer() & ~DISABLE_PLAYERS_FLAG );

                        }

                        return;

                    }
                    else {

                        if (IOMgr::kb_check(sf::Keyboard::A)) {

                            QAO_DELETE( GlobalSt.player[0] );
                            QAO_DELETE( GlobalSt.player[1] );

                            GlobalSt_Mut.player[0] = nullptr;
                            GlobalSt_Mut.player[1] = nullptr;

                            ClearConsole();

                            QAO_CREATE( so::Greeter() );

                            return;

                        }
                        else if (IOMgr::kb_check(sf::Keyboard::Q)) {
                        
                            QAO_DELETE( GlobalSt.player[0] );
                            QAO_DELETE( GlobalSt.player[1] );

                            GlobalSt_Mut.player[0] = nullptr;
                            GlobalSt_Mut.player[1] = nullptr;

                            GlobalSt_Mut.break_program_loop = true;

                            return;

                        }

                    }

                }

                // If next move is ready:
                if (flow::NextMoveReady()) {

                    GameMove move = flow::PopNextMove();

                    GameState new_state = flow::GetCurrentState().test_move(move);

                    flow::SetCurrentState(new_state);

                    if (MinimaxTree::test_for_victory(new_state, flow::GetCurrentPlayer())) {

                        // Player won the game:
                        victor = flow::GetCurrentPlayer();
                        flow::SetCurrentPlayer( DISABLE_PLAYERS_FLAG );
                        cooldown = MAX_COOLDOWN;

                        std::cout << "========================================\n";
                        std::cout << "| Player " << (victor + 1) << " has won!                    |\n";
                        std::cout << "| Press A to play again.               |\n";
                        std::cout << "| Press Q to quit.                     |\n";
                        std::cout << "========================================\n";
                
                        if (!GlobalSt.output_file_path.empty() && flow::OutputQueueHasData()) {
                        
                            std::cout << "Writing game history to output file...\n";
                            flow::WriteOutputQueueToFile(GlobalSt.output_file_path.c_str());
                            std::cout << "Done!";
                            flow::ResetOutputQueue();

                        }

                    }
                    else {

                        // Game carries on:
                        UINT npl = ReversePlayerIndex( flow::GetCurrentPlayer() );
                        flow::SetCurrentPlayer( npl | DISABLE_PLAYERS_FLAG );
                        cooldown = MAX_COOLDOWN;

                    }

                }
                break;

            default:
                assert(0);
                break;

            }

        }

        void GameController::event_draw_1() {
        
            UINT icd = MAX_COOLDOWN - cooldown; // Inverse cooldown

            // Draw the board:
            if (icd == MOVE_DURATION || !initial_render || fast_mode || GlobalSt.redraw_needed) {

                DrawBoard(board_texture, 0.0, 0.0);
                board_texture.display();

                initial_render = true;
                GlobalSt_Mut.redraw_needed = false;

            }

            draw_set_color(sf::Color::White);
            draw_set_alpha(255);
            draw_texture(0.0, 0.0, board_texture.getTexture());

            // Draw pawns:
            double interpolation_amount = (icd < MOVE_DURATION) ? (double(icd) / MOVE_DURATION) : 1.0;

            GameState scurr = flow::GetCurrentState();
            GameState sprev = flow::GetLastState();

            for (UINT player = 0; player < 2; player += 1)
                for (UINT pawn = 0; pawn < 2; pawn += 1) {
                        
                    if (scurr.get_pos(player, pawn) == Char2d{-1, -1}) continue;

                    DrawPawn(0.0, 0.0, sprev, scurr, player, pawn, _IA);

                }

            // Instantiate a CellHighlight object if needed:
            if (icd == MOVE_DURATION) {
            
                GameMove lm = flow::GetLastMove();

                double tx = (lm.bld_coord.x + 1) * CELL_SIZE;
                double ty = (lm.bld_coord.y + 1) * CELL_SIZE;

                QAO_CREATE( so::CellHighlight(tx, ty, CELL_SIZE, CELL_SIZE, BUILD_DURATION) );

            }

        }

        void GameController::event_draw_g() {
        
            char buff[100];

            sf::String str;

            if (victor != NO_VICTOR) {

                snprintf(buff, sizeof(buff), "Player %d has won! Press A to play again or Q to quit.", int(victor + 1));

                str = buff;

                draw_set_alpha(255);
                draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);
                draw_set_color((victor == 0) ? COL_ORANGE : COL_PURPLE);
                draw_text(12.0, 16.0, str, 24);

            }
            else if (sbs_mode && cooldown == 1) {
            
                snprintf(buff, sizeof(buff), "Press N to advance the game or S to turn off S-b-S mode.");

                str = buff;

                draw_set_alpha(255);
                draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);
                draw_set_color(sf::Color::Yellow);
                draw_text(12.0, 16.0, str, 24);

            }
            
            float yy = IOMgr::view_get_raw(IOMgr::VIEW_GUI).getSize().y - 40.f;

            snprintf(buff, sizeof(buff), "[F] - Fast mode; [C] - Compact mode; [S] - Step-by-step mode. [N] - Advance in S-b-s mode.");

            str = buff;

            draw_set_alpha(255);
            draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);
            draw_set_color(sf::Color::White);
            draw_text(12.0, yy, str, (compact_mode ? 16 : 20));

        }

    }

    #undef CELL_SIZE
    #undef _IA

}