
#include "pch.h"

#include "CellHighlight.hpp"
#include "QAO-Module.hpp"
#include "ObjectTID.hpp"
#include "Gr-Module.hpp"

#include <cmath>

using namespace etf_santorini_bj150087d::gr;

namespace etf_santorini_bj150087d {

    namespace so {

        CellHighlight::CellHighlight(double x, double y, double w, double h, UINT duration)
            : qao::Base(TID::CellHighlight, -10, "CellHighlight")
            , x(x), y(y), w(w), h(h)
            , cnt_max(duration) {
        
            cnt_curr = 0;

        }

        void CellHighlight::event_update() {
            
            if (cnt_curr == cnt_max) {
                QAO_DELETE(this);
                return;
            }

            cnt_curr += 1;

        }

        void CellHighlight::event_draw_1() {

            static const double PI = 3.14159265358979323846;

            double tetha = PI * double(cnt_curr) / double(cnt_max);

            draw_set_alpha( sf::Uint8(128.0 * std::sin(tetha)) );

            draw_set_color(sf::Color::White);

            draw_rectangle(x, y, w, h);

        }

    }

}