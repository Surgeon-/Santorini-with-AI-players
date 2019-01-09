
#include "pch.h"

#include "Utility"
#include "QAO-Module.hpp"
#include "HelperStructs.hpp"
#include "ComputerPlayer.hpp"
#include "GlobalStore.hpp"
#include "ObjectTID.hpp"
#include <SFML/System.hpp>
#include "GameFlowFunc.hpp"
#include "MinimaxTree.hpp"
#include "EstimateFunc.hpp"
#include "Gr-Module.hpp"
#include "Random.hpp"

#include <cassert>
#include <chrono>
#include <iostream> // TEMP
#include <stdexcept>
#include <string>

using namespace etf_santorini_bj150087d::gr;

namespace etf_santorini_bj150087d {

    namespace so {

        #define WORKER_STATUS_OK    0
        #define WORKER_STATUS_ERROR 1

        ComputerPlayer::ComputerPlayer(UINT player_index, UINT level, UINT max_depth, bool use_pruning, bool random_choice)
            : qao::Base(TID::ComputerPlayer, -5, "ComputerPlayer")
            , sem(0)
            , worker(worker_job, qao::Ptr<ComputerPlayer>{this})
            , level(level)
            , player_index(player_index)
            , max_depth(max_depth)
            , use_pruning(use_pruning)
            , random_choice(random_choice)
            , worker_message("")
            , worker_status(WORKER_STATUS_OK) {
        
            best_move.reset();

        }

        ComputerPlayer::~ComputerPlayer() {
        
            assert(worker.joinable());

            end_job = true;
            sem.signal();
            worker.join();
        
        }

        void ComputerPlayer::event_update() {

            if (flow::GetCurrentPlayer() != player_index || flow::NextMoveReady()) return;

            switch (flow::GetCurrentPhase()) {

            case flow::PHASE_PLAYER_SETUP:

                for (UINT i = 0; i < 2; i += 1) {
                
                    GameState curr = flow::GetCurrentState();

                    RETRY:

                    Char2d newpos = Char2d{ char(Random(4)), char(Random(4)) };

                    for (UINT j = 0; j < 2; j += 1)
                        for (UINT k = 0; k < 2; k += 1)
                            if (curr.get_pos(j, k) == newpos) goto RETRY;

                    flow::SetPawnPosition(player_index, i, newpos);

                    flow::NotifyPlace(player_index, (level == 0) ? "CPU" : "Adv-CPU", newpos, i);

                }

                break;

            case flow::PHASE_PLAY:

                if (worker_busy) return;

                if (!best_move.has_value()) { // Should begin new calculation
            
                    // best_move.reset();
                    worker_busy = true;
                    sem.signal();

                }
                else { // Calculation ended
            
                    if (worker_status == WORKER_STATUS_ERROR) {
                        throw std::exception(worker_message.c_str());
                    }

                    flow::NotifyMove(player_index, (level == 0) ? "CPU" : "Adv-CPU",  best_move.value(), time_elapsed);

                    flow::PushNextMove(best_move.value());

                    best_move.reset();

                }

                break;

            default:
                assert(0);
                break;

            }
        
        }

        void ComputerPlayer::worker_job(qao::Ptr<ComputerPlayer> player_obj) {
        
            while (true) {

                player_obj->worker_busy = false;

                player_obj->sem.wait();

                if (player_obj->end_job) break;

                auto time1 = std::chrono::steady_clock::now();

                GameMove move;
                
                try {

                    if (player_obj->level == 0) { // Basic AI

                        move = MinimaxTree::calc_next_move( flow::GetCurrentState()
                                                          , player_obj->player_index
                                                          , player_obj->max_depth
                                                          , SimpleEstimate
                                                          , player_obj->use_pruning
                                                          , player_obj->random_choice
                                                          ) ;

                    }
                    else { // Competitive AI
                
                        move = MinimaxTree::calc_next_move_ext( flow::GetCurrentState()
                                                              , player_obj->player_index
                                                              , player_obj->max_depth
                                                              , CompetitiveEstimate
                                                              , player_obj->use_pruning
                                                              , player_obj->random_choice
                                                              ) ;

                    }

                }
                catch (std::exception & ex) {

                    player_obj->worker_message = std::string(ex.what());
                    player_obj->worker_status  = WORKER_STATUS_ERROR;

                }
                catch (...) {

                    player_obj->worker_message = std::string("Unknown exception.");
                    player_obj->worker_status  = WORKER_STATUS_ERROR;

                }

                auto time2 = std::chrono::steady_clock::now();

                player_obj->time_elapsed = int( std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() );
                player_obj->best_move.emplace(move);

            }

        }

        void ComputerPlayer::event_draw_g() {
        
            if (flow::GetCurrentPlayer() != player_index || flow::NextMoveReady()) return;

            char buff[32];

            snprintf(buff, sizeof(buff), "Player %d (%s): ", int(player_index + 1), (level == 0) ? "CPU" : "Adv-CPU");

            sf::String str = buff;

            auto snd30 = qao::step_number() / 30u;

            str += (snd30 % 4 == 0) ? "Thinking"   :
                   (snd30 % 4 == 1) ? "Thinking."  :
                   (snd30 % 4 == 2) ? "Thinking.." : "Thinking...";

            draw_set_alpha(255);
            draw_set_font(GlobalSt.fonts[FontID::TrebuchetMS]);
            draw_set_color((player_index == 0) ? COL_ORANGE : COL_PURPLE);

            draw_text(12.0, 16.0, str, 24);

        }

        #undef WORKER_STATUS_OK   
        #undef WORKER_STATUS_ERROR

    }

}