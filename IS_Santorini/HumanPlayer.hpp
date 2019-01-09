#pragma once

#include "QAO-Module.hpp"
#include "Utility.hpp"
#include "HelperStructs.hpp"
#include "Semaphore.hpp"

#include <chrono>

namespace etf_santorini_bj150087d {

    namespace so {

        class HumanPlayer : public qao::Base {

        public:

            HumanPlayer(UINT player_index);

            ~HumanPlayer();

        protected:

            HumanPlayer * clone() const override { return nullptr; }

            //void event_prep();
            void event_update() override;
            void event_draw_1() override;
            //void event_draw_2();
            void event_draw_g() override;
            //void event_frame_end();

        private:

            struct State {
            
                enum Enum {
                
                    Idle, ClickedPawn, ClickedMove

                };

            };

            UINT player_index;
            State::Enum state; 
            bool skipped_last;

            UINT pawn_index;
            Char2d pwn_coord, mov_coord, bld_coord;

            std::chrono::time_point<std::chrono::steady_clock> turn_begin_time;

            void exec_setup();
            void exec_idle();
            void exec_clpawn();
            void exec_clmove();

            void draw_setup();
            void draw_idle();
            void draw_clpawn();
            void draw_clmove();

        };

    }

}