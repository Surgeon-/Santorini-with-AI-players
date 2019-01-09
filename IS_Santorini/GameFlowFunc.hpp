#pragma once

#include "Utility.hpp"
#include "HelperStructs.hpp"

namespace etf_santorini_bj150087d {

    namespace flow {

        // Current player:

        UINT GetCurrentPlayer();

        void SetCurrentPlayer(UINT ind);

        // Next move:

        bool NextMoveReady();

        GameMove PopNextMove();

        void PushNextMove(const GameMove & move);

        GameMove GetLastMove();

        // Game phase:

        constexpr UINT PHASE_PLAYER_SETUP = 0;
        constexpr UINT PHASE_PLAY         = 1;

        UINT GetCurrentPhase();

        void SetCurrentPhase(UINT phase);

        // Game state:

        GameState GetCurrentState();

        void SetCurrentState(const GameState & state);

        GameState GetLastState();

        void SetPawnPosition(UINT player_index, UINT pawn_index, Char2d pos);

        // Reset whole game:

        void CompleteReset();

        // Notify:

        void NotifyPlace(UINT player_index, const char * name, Char2d pos, UINT pawn_index);

        void NotifyMove(UINT player_index, const char * name, const GameMove & move, long time);

        // File I/O:
        void LoadStateFromFile(const char * path);

        void ResetOutputQueue();

        bool OutputQueueHasData();

        void WriteOutputQueueToFile(const char * path);

    }

}