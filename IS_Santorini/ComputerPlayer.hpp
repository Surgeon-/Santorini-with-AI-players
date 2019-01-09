#pragma once

#include "QAO-Module.hpp"
#include "Utility.hpp"
#include "Semaphore.hpp"

#include <thread>
#include <optional>

namespace etf_santorini_bj150087d {

    namespace so {
    
        class ComputerPlayer : public qao::Base {
        
        public:

            ComputerPlayer(UINT player_index, UINT level = 0, UINT max_depth = 1, bool use_pruning = false, bool random_choice = false);

            ~ComputerPlayer();

        protected:

            ComputerPlayer * clone() const override { return nullptr; }

            //void event_prep();
            void event_update() override;
            //void event_draw_1();
            //void event_draw_2();
            void event_draw_g() override;
            //void event_frame_end();

        private:

            UINT player_index;
            UINT max_depth;
            UINT level;
            bool use_pruning;
            bool random_choice;

            // Worker thread stuff:
            ::etf_santorini_bj150087d::util::Semaphore sem;
            bool worker_busy;
            bool end_job;
            std::thread worker;
            std::optional<GameMove> best_move;
            int time_elapsed;
            std::string worker_message;
            int worker_status;

            static void worker_job(qao::Ptr<ComputerPlayer> player_obj);
        
        };

    }

}