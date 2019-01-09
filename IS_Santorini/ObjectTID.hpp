#pragma once

// Note: TID = Type Identificator

namespace etf_santorini_bj150087d {

    namespace so { // Start SO namespace

        struct TID {

            enum Enum {

                Null = 0

                , IOMgr             = 1
                , GameController    = 2
                , HumanPlayer       = 3
                , ComputerPlayer    = 4
                , Greeter           = 5
                , CellHighlight     = 6

            };

        };

    } // End SO namespace

}