#pragma once

#include "QAO-Module.hpp"
#include "Utility.hpp"

namespace etf_santorini_bj150087d {

    namespace so {

        class CellHighlight : public qao::Base {

        public:

            CellHighlight(double x, double y, double w, double h, UINT duration);

        protected:

            CellHighlight * clone() const override { return nullptr; }

            //void event_prep();
            void event_update() override;
            void event_draw_1() override;
            //void event_draw_2();
            //void event_draw_g() override;
            //void event_frame_end();

        private:

            UINT cnt_curr, cnt_max;

            double x, y, w, h;

        };

    }

}