
#include "pch.h"

#include "Utility.hpp"
#include "EstimateFunc.hpp"
#include "HelperStructs.hpp"

#include <algorithm>
#include <stdexcept>

namespace etf_santorini_bj150087d {

    namespace {
    
        bool IsAdjecent(Char2d pos1, Char2d pos2) {

            if (std::abs(pos2.x - pos1.x) > 1) return false;

            if (std::abs(pos2.y - pos1.y) > 1) return false;

            return true;

        }

        int ManhattanDist(Char2d pos1, Char2d pos2) {
        
            return (std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y));

        }

        int ActualDist(Char2d pos1, Char2d pos2) {
        
            int rv = 0;

            while (pos1.x != pos2.x && pos1.y != pos2.y) {
            
                pos1.x += (pos1.x < pos2.x) ? 1 : -1;
                pos1.y += (pos1.y < pos2.y) ? 1 : -1;

                rv += 1;

            }

            rv += ManhattanDist(pos1, pos2);

            return rv;

        }
    
    }

    int SimpleEstimate(const GameState & prev_state, const GameState & curr_state, const GameMove & move, UINT just_played, UINT for_player) {
    
        // Mandatory condition for this simple function:
        if (just_played != for_player) throw std::logic_error("SimpleEstimate(...) - just_played != for_player.");

        int m = int( prev_state.at(UINT(move.pos_coord.x), UINT(move.pos_coord.y)) );

        int self_dist0 = ManhattanDist(prev_state.get_pos(for_player, 0), move.bld_coord);
        int self_dist1 = ManhattanDist(prev_state.get_pos(for_player, 1), move.bld_coord);

        int enemy_dist0 = ManhattanDist(prev_state.get_pos(ReversePlayerIndex(for_player), 0), move.bld_coord);
        int enemy_dist1 = ManhattanDist(prev_state.get_pos(ReversePlayerIndex(for_player), 1), move.bld_coord);

        int l = int( prev_state.at(UINT(move.bld_coord.x), UINT(move.bld_coord.y)) );

        l *= (std::min(self_dist0, self_dist1) - std::min(enemy_dist0, enemy_dist1));

        return (m + l);

    }

    namespace {
    
        int PositionEstimate(const GameState & curr_state, UINT for_player) {
        
            int rv = 0;

            for (UINT pawn = 0; pawn < 2; pawn += 1)
                for (char xoff = -1; xoff <= 1; xoff += 1)
                    for (char yoff = -1; yoff <= 1; yoff += 1) {

                        Char2d pos = curr_state.get_pos(for_player, pawn);

                        char xx = pos.x + xoff;
                        if (xx < 0 || xx >= 5) continue;

                        char yy = pos.y + yoff;
                        if (yy < 0 || yy >= 5) continue;

                        char myval = curr_state.at(UINT(pos.x), UINT(pos.y));
                        char xyval = curr_state.at(UINT(xx), UINT(yy));

                        if (xoff == 0 && yoff == 0) {
                            rv += xyval * 10;
                        }
                        else {

                            if (xyval > myval + 1)
                                rv -= (xyval - myval) * 3;
                            else
                                rv += xyval * ((xyval == 4) ? 2 : 5);

                        }

                    }

            return rv;

        }

        // Good:
        int PositionEstimate2(const GameState & curr_state, UINT for_player) {

            static const int worth[5] = { 0, 4, 10, 24, 4 };

            int rv = 0;

            bool w = false;

            for (UINT pawn = 0; pawn < 2; pawn += 1)
                for (char xoff = -1; xoff <= 1; xoff += 1)
                    for (char yoff = -1; yoff <= 1; yoff += 1) {

                        Char2d pos = curr_state.get_pos(for_player, pawn);

                        char xx = pos.x + xoff;
                        if (xx < 0 || xx >= 5) continue;

                        char yy = pos.y + yoff;
                        if (yy < 0 || yy >= 5) continue;

                        char myval = curr_state.at(UINT(pos.x), UINT(pos.y));
                        char xyval = curr_state.at(UINT(xx), UINT(yy));

                        if (xoff == 0 && yoff == 0) {
                            rv += xyval * 20;
                        }
                        else {

                            if (myval == 2 and xyval == 3) {
                                if (w) return 2000;
                                w = true;
                            }

                            if (xyval > myval + 1)
                                rv -= (xyval - myval) * 6;
                            else
                                rv += worth[xyval];

                        }

                    }

            return rv;

        }

        int DistanceEstimate(const GameState & curr_state, UINT just_played, UINT for_player) {

            int rv = 0;

            UINT opponent = ReversePlayerIndex(for_player);

            for (UINT pawn = 0; pawn < 2; pawn += 1) {

                Char2d mypos = curr_state.get_pos(for_player, pawn);

                rv += std::min( ActualDist(mypos, curr_state.get_pos(opponent, 0u)) - 1
                              , ActualDist(mypos, curr_state.get_pos(opponent, 1u)) - 1
                              ) * 10 * (curr_state.at(UINT(mypos.x), UINT(mypos.y)) + 1);

            }

            if (just_played == for_player) rv /= 2;
            //if (just_played != for_player) rv *= 2;

            return rv;

        }

        int DistanceEstimate2(const GameState & curr_state, UINT just_played, UINT for_player) {

            int rv = 0;

            UINT opponent = ReversePlayerIndex(for_player);

            /*for (UINT pawn = 0; pawn < 2; pawn += 1) {

                Char2d mypos = curr_state.get_pos(for_player, pawn);

                rv += std::min( ActualDist(mypos, curr_state.get_pos(opponent, 0u)) - 1
                              , ActualDist(mypos, curr_state.get_pos(opponent, 1u)) - 1
                              ) * 10 * (curr_state.at(UINT(mypos.x), UINT(mypos.y)) + 1);

            }*/

            for (UINT pawn = 0; pawn < 2; pawn += 1) { // Loop over opponent's pawns

                Char2d oppos = curr_state.get_pos(opponent, pawn);

                int mindist = std::min( ActualDist(oppos, curr_state.get_pos(for_player, 0u))
                                      , ActualDist(oppos, curr_state.get_pos(for_player, 1u))
                                      ) ;

                rv -= mindist * curr_state.at(UINT(oppos.x), UINT(oppos.y)) * 10;

            }

            //rv *= 2;

            if (just_played == for_player) rv /= 2;
            //if (just_played != for_player) rv /= 2;

            return rv;

        }

        int BuildEstimate(const GameState & prev_state, const GameMove & move, UINT for_player) {
        
            static const int values_1[4] = {1, 2, 3, 8};
            static const int values_2[4] = {2, 4, 6, 1};

            int rv = 0;

            UINT opponent = ReversePlayerIndex(for_player);

            if (IsAdjecent(move.bld_coord, prev_state.get_pos(opponent, 0u)) ||
                IsAdjecent(move.bld_coord, prev_state.get_pos(opponent, 1u))) {

                rv += 2 * values_1[ prev_state.at(UINT(move.bld_coord.x), UINT(move.bld_coord.y)) ];

            }
            else {
            
                rv += 2 * values_2[ prev_state.at(UINT(move.bld_coord.x), UINT(move.bld_coord.y)) ];

            }

            return rv;

        }

        // Good:
        int ReachabilityEstimate(const GameState & curr_state, UINT for_player) {
        
            int rv = 0;

            bool myreach[5][5];
            bool opreach[5][5];

            for (char x = 0; x < 5; x += 1)
                for (char y = 0; y < 5; y += 1) {
                    myreach[x][y] = false;
                    opreach[x][y] = false;
                }

            // My reach:
            for (UINT pawn = 0; pawn < 2; pawn += 1) {
            
                Char2d pos = curr_state.get_pos(for_player, pawn);

                for (char xoff = -1; xoff <= 1; xoff += 1)
                    for (char yoff = -1; yoff <= 1; yoff += 1) {

                        char xx = pos.x + xoff;
                        if (xx < 0 || xx >= 5) continue;

                        char yy = pos.y + yoff;
                        if (yy < 0 || yy >= 5) continue;

                        if ( (xoff == 0 && yoff == 0) || 
                             (curr_state.at(UINT(xx), UINT(yy)) <= 1 + curr_state.at(UINT(pos.x), UINT(pos.y))) )
                            myreach[xx][yy] = true;

                    }

            }

            // Opponent's reach:
            for (UINT pawn = 0; pawn < 2; pawn += 1) {
            
                Char2d pos = curr_state.get_pos(ReversePlayerIndex(for_player), pawn);

                for (char xoff = -1; xoff <= 1; xoff += 1)
                    for (char yoff = -1; yoff <= 1; yoff += 1) {

                        char xx = pos.x + xoff;
                        if (xx < 0 || xx >= 5) continue;

                        char yy = pos.y + yoff;
                        if (yy < 0 || yy >= 5) continue;

                        if ( (xoff == 0 && yoff == 0) || 
                             (curr_state.at(UINT(xx), UINT(yy)) <= 1 + curr_state.at(UINT(pos.x), UINT(pos.y))) )
                            opreach[xx][yy] = true;

                    }

            }

            // Compare:
            //               OLD VALUES: { 1,  8, 30, 1000, 0 };
            static const int values[5] = { 8, 10, 30, 1000, 0 };

            for (char x = 0; x < 5; x += 1)
                for (char y = 0; y < 5; y += 1) {
                
                    char height = curr_state.at(UINT(x), UINT(y));

                    if (myreach[x][y] && !opreach[x][y]) {
                        rv += values[height];
                    }
                    else if (!myreach[x][y] && opreach[x][y]) {
                        rv -= values[height];
                    }

                }

            // Return:
            return rv;

        }

    }

    int CompetitiveEstimate(const GameState & prev_state, const GameState & curr_state, const GameMove & move, UINT just_played, UINT for_player) {
    
        UINT opponent = ReversePlayerIndex(for_player);

        int rv = 0;

        rv += (PositionEstimate2(curr_state, for_player) - PositionEstimate2(curr_state, opponent));
        rv += ReachabilityEstimate(curr_state, for_player); // Function is symmetrical      

        return rv;

    }

}