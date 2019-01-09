#pragma once

#include "SlabManager.hpp"
#include "Utility.hpp"

namespace etf_santorini_bj150087d {

    namespace qao {
    
        using Index = util::SlabManager::Index;

        using Uid   = size_t;

        const Uid NULL_UID = Uid(0);

        using Step  = unsigned long;

        struct Event {

            enum Enum {

                NoEvent,
                Prep,
                PreUpdate,
                Update,
                PostUpdate,
                Draw1,
                Draw2,
                DrawGUI,
                FrameEnd,

                Count

            };

        };

        const int MIN_PRIORITY = util::min_val<int>();
        const int MAX_PRIORITY = util::max_val<int>();

    }

}
