#pragma once

namespace etf_santorini_bj150087d {

    namespace gr {
    
        class Percent {
        
        public:

            double value;

            explicit Percent(double value = 0.0)
                : value(value) { }

        };

        class Transform2d {
        
        public:

            double origin_xoff;
            double origin_yoff;
            double x_scale;
            double y_scale;
            double rotation_deg;
        
            bool is_percent_based() const { return percent_based; };

            Transform2d(double origin_xoff = 0.0,
                        double origin_yoff = 0.0,
                        double x_scale = 1.0,
                        double y_scale = 1.0,
                        double rotation_deg = 0.0
                        )
                : origin_xoff(origin_xoff)
                , origin_yoff(origin_yoff)
                , x_scale(x_scale)
                , y_scale(y_scale) 
                , rotation_deg(rotation_deg)
                , percent_based(false) { }

            Transform2d(Percent origin_xoff,
                        Percent origin_yoff,
                        double x_scale = 1.0,
                        double y_scale = 1.0,
                        double rotation_deg = 0.0
                        )
                : origin_xoff(origin_xoff.value)
                , origin_yoff(origin_yoff.value)
                , x_scale(x_scale)
                , y_scale(y_scale) 
                , rotation_deg(rotation_deg)
                , percent_based(true) { }

        private:

            bool percent_based;

        };
    
    }

}