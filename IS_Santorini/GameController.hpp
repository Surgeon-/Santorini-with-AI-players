#pragma once

#include "QAO-Module.hpp"
#include "Utility.hpp"
#include "GameFlowFunc.hpp"
#include "Gr-Module.hpp"

#include <thread>
#include <optional>

namespace etf_santorini_bj150087d {

    namespace so {

        class GameController : public qao::Base {

        public:

            GameController();

            ~GameController();

        protected:

            GameController * clone() const override { return nullptr; }

            //void event_prep();
            void event_update() override;
            void event_draw_1() override;
            //void event_draw_2();
            void event_draw_g() override;
            //void event_frame_end();

        private:

            static const UINT MOVE_DURATION  = 40;
            static const UINT BUILD_DURATION = 20;
            static const UINT MAX_COOLDOWN   = MOVE_DURATION + BUILD_DURATION;
            static const UINT NO_VICTOR      = DISABLE_PLAYERS_FLAG;

            UINT cooldown;
            UINT victor = NO_VICTOR;

            sf::RenderTexture board_texture;
            bool initial_render;
            bool fast_mode;
            bool compact_mode;
            bool sbs_mode;

        };

    }

}