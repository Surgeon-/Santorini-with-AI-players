#pragma once

/* Class for storing variables that need to be globally visible
* in a centralized place.
* Macros GlobalSt and GlobalSt_Mut macros are provided for easy
* access for reading and writing (_Mut variant).
* Example: auto nick = GlobalSt.player_nickname;
* Example: GlobalSt_Mut.player_nickname = "xXx_N00bSlayer420_xXx";
*/

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

#include "QAO-Module.hpp"

// Forward declarations:
// ---------------------

namespace etf_santorini_bj150087d {

    class GlobalStore {

    public:

        // CONSTANTS: /////////////////////////////////////////

        // DATA: //////////////////////////////////////////////
        bool break_program_loop;

        std::vector<sf::Font> fonts;

        qao::GenericPtr player[2];

        std::string input_file_path;
        std::string output_file_path;

        bool redraw_needed;

        // ACCESS: ////////////////////////////////////////////
        inline
        static const GlobalStore & get_singleton();

        inline
        static GlobalStore & get_singleton_mutable();

    private:

        GlobalStore() { }

        static GlobalStore singleton;

    };

    #define GlobalSt     etf_santorini_bj150087d::GlobalStore::get_singleton()
    #define GlobalSt_Mut etf_santorini_bj150087d::GlobalStore::get_singleton_mutable()

    ///////////////////////////////////////////////////////////
    inline
    const GlobalStore & GlobalStore::get_singleton() {

        return const_cast<const GlobalStore &>(singleton);

    }

    inline
    GlobalStore & GlobalStore::get_singleton_mutable() {

        return const_cast<GlobalStore &>(singleton);

    }

}
