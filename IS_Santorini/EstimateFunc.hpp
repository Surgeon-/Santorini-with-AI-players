#pragma once

#include "Utility.hpp"
#include "HelperStructs.hpp"

namespace etf_santorini_bj150087d {

    using EstimateFn = int (*)(const GameState & prev_state, const GameState & curr_state, const GameMove & move, UINT just_played, UINT for_player);

    int SimpleEstimate(const GameState & prev_state, const GameState & curr_state, const GameMove & move, UINT just_played, UINT for_player);

    int CompetitiveEstimate(const GameState & prev_state, const GameState & curr_state, const GameMove & move, UINT just_played, UINT for_player);

}