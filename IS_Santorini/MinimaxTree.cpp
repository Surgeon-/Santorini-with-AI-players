
#include "pch.h"

#include "Utility.hpp"
#include "MinimaxTree.hpp"
#include "HelperStructs.hpp"

#include "Random.hpp"
#include <algorithm>

#include <deque>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

#include <iostream> // TEMP !!!

namespace etf_santorini_bj150087d {

    // MIN / MAX (normal):
    int MinimaxTree::node_min(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse) {

        UINT curr_player = ((node->depth % 2 == 0) ? player_index : ReversePlayerIndex(player_index));
        UINT just_played = ReversePlayerIndex(curr_player);

        // Test for victory:
        if (node->test_victory(just_played)) {

            if (!no_collapse) node->collapse();

            if (just_played == player_index) { // Globally current player wins
                node->value = VAL_WIN - node->depth;
            }
            else { // Opposite player wins
                node->value = VAL_LOSE + node->depth;
            }

            return node->value;

        }

        // Test for depth limit:
        if (node->depth == max_depth) {

            assert( parent != nullptr && used_move != nullptr );

            node->value = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            return node->value;

        }

        // Use MiniMax:
        node->value = util::max_val<int>();

        node->generate_children(curr_player);

        if (use_pruning && parent != nullptr) {

            node->alpha = parent->alpha;
            node->beta  = parent->beta;

        }

        for (auto & elem : node->children) {
        
            int tmpval = node_max(elem.node.get(), node, player_index, &elem.move, max_depth, estimate_fn, use_pruning, false);

            if (tmpval < node->value) node->value = tmpval;

            if (use_pruning && parent != nullptr) {
            
                if (tmpval <= node->alpha) break;

                if (tmpval < node->beta) node->beta = tmpval;

            }

        }

        // Collapse to conserve RAM:
        if (!no_collapse) node->collapse();

        // Return:
        return node->value;

    }

    int MinimaxTree::node_max(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse) {
    
        UINT curr_player = ((node->depth % 2 == 0) ? player_index : ReversePlayerIndex(player_index));
        UINT just_played = ReversePlayerIndex(curr_player);

        // Test for victory:
        if (node->test_victory(just_played)) {

            if (!no_collapse) node->collapse();

            if (just_played == player_index) { // Globally current player wins
                node->value = VAL_WIN - node->depth;
            }
            else { // Opposite player wins
                node->value = VAL_LOSE + node->depth;
            }

            return node->value;

        }

        // Test for depth limit:
        if (node->depth == max_depth) {

            assert( parent != nullptr && used_move != nullptr );

            node->value = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            return node->value;

        }

        // Use MiniMax:
        node->value = util::min_val<int>();

        node->generate_children(curr_player);

        if (use_pruning && parent != nullptr) {

            node->alpha = parent->alpha;
            node->beta  = parent->beta;

        }

        for (auto & elem : node->children) {

            int tmpval = node_min(elem.node.get(), node, player_index, &elem.move, max_depth, estimate_fn, use_pruning, false);

            if (tmpval > node->value) node->value = tmpval;

            if (use_pruning && parent != nullptr) {

                if (tmpval >= node->beta) break;

                if (tmpval > node->alpha) node->alpha = tmpval;

            }

        }

        // Collapse to conserve RAM:
        if (!no_collapse) node->collapse();

        // Return:
        return node->value;

    }

    // MIN / MAX (extended):
    int MinimaxTree::node_min_ext(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse) {

        UINT curr_player = ((node->depth % 2 == 0) ? player_index : ReversePlayerIndex(player_index));
        UINT just_played = ReversePlayerIndex(curr_player);

        // Test for victory:
        if (node->test_victory(just_played)) {

            if (!no_collapse) node->collapse();

            if (just_played == player_index) { // Globally current player wins
                node->value = VAL_WIN - node->depth;
            }
            else { // Opposite player wins
                node->value = VAL_LOSE + node->depth;
            }

            return node->value;

        }

        // Test for depth limit:
        if (node->depth == max_depth) {

            assert( parent != nullptr && used_move != nullptr );

            node->value = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            return node->value;

        }

        // Use MiniMax:
        node->value = util::max_val<int>();

        node->generate_children(curr_player);

        if (use_pruning && parent != nullptr) {

            node->alpha = parent->alpha;
            node->beta  = parent->beta;

        }

        for (auto & elem : node->children) {

            int tmpval = node_max_ext(elem.node.get(), node, player_index, &elem.move, max_depth, estimate_fn, use_pruning, false);

            if (tmpval < node->value) node->value = tmpval;

            if (use_pruning && parent != nullptr) {

                if (tmpval <= node->alpha) break;

                if (tmpval < node->beta) node->beta = tmpval;

            }

        }

        if (parent != nullptr) {

            int estim_this = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            node->value = std::min(estim_this, node->value);

        }

        // Collapse to conserve RAM:
        if (!no_collapse) node->collapse();

        // Return:
        return node->value;

    }

    int MinimaxTree::node_max_ext(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse) {

        UINT curr_player = ((node->depth % 2 == 0) ? player_index : ReversePlayerIndex(player_index));
        UINT just_played = ReversePlayerIndex(curr_player);

        // Test for victory:
        if (node->test_victory(just_played)) {

            if (!no_collapse) node->collapse();

            if (just_played == player_index) { // Globally current player wins
                node->value = VAL_WIN - node->depth;
            }
            else { // Opposite player wins
                node->value = VAL_LOSE + node->depth;
            }

            return node->value;

        }

        // Test for depth limit:
        if (node->depth == max_depth) {

            assert( parent != nullptr && used_move != nullptr );

            node->value = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            return node->value;

        }

        // Use MiniMax:
        node->value = util::min_val<int>();

        node->generate_children(curr_player);

        if (use_pruning && parent != nullptr) {

            node->alpha = parent->alpha;
            node->beta  = parent->beta;

        }

        for (auto & elem : node->children) {

            int tmpval = node_min_ext(elem.node.get(), node, player_index, &elem.move, max_depth, estimate_fn, use_pruning, false);

            if (tmpval > node->value) node->value = tmpval;

            if (use_pruning && parent != nullptr) {

                if (tmpval >= node->beta) break;

                if (tmpval > node->alpha) node->alpha = tmpval;

            }

        }

        if (parent != nullptr) {

            int estim_this = estimate_fn(parent->state, node->state, *used_move, just_played, player_index);

            node->value = std::max(estim_this, node->value);

        }

        // Collapse to conserve RAM:
        if (!no_collapse) node->collapse();

        // Return:
        return node->value;

    }

    bool MinimaxTree::test_for_victory(const GameState & state, UINT just_played) {
    
        const Node node{0, state};

        return (node.test_victory(just_played));

    }

    GameMove MinimaxTree::calc_next_move(const GameState & current_state, UINT player_index,
                                         UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool random_choice) {

        Node root{0, current_state};

        int max = node_max(&root, nullptr, player_index, nullptr, max_depth, estimate_fn, use_pruning, true);
        // Note: node_max won't collapse the root

        std::sort(root.children.rbegin(), root.children.rend());

        UINT count = 0;
        for (auto & elem : root.children) {
        
            if (elem.node->value == max)
                count += 1;
            else
                break;

        }

        UINT pos = random_choice ? Random(count - 1) : 0;

        return (root.children[pos].move);
    
    }

    GameMove MinimaxTree::calc_next_move_ext(const GameState & current_state, UINT player_index,
                                             UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool random_choice) {

        // Old impl:
        Node root{0, current_state};

        int max = node_max_ext(&root, nullptr, player_index, nullptr, max_depth, estimate_fn, use_pruning, true);
        // Note: node_max_ext won't collapse the root

        std::sort(root.children.rbegin(), root.children.rend());

        UINT count = 0;
        for (auto & elem : root.children) {

            if (elem.node->value == max)
                count += 1;
            else
                break;

        }

        UINT pos = random_choice ? Random(count - 1) : 0;

        return (root.children[pos].move);

        ///////////////////////////////////////////////////////////////////////

        /*int best_val = util::min_val<int>();
        GameMove best_move{};

        Node root{0, current_state};

        int max = node_max_ext(&root, nullptr, player_index, nullptr, max_depth, estimate_fn, use_pruning, true);
        // Note: node_max_ext won't collapse the root

        std::sort(root.children.rbegin(), root.children.rend());

        UINT count = 0;
        for (auto & item : root.children) {

            if (std::abs(item.node->value - max) <= 10) // Within 10 of max
                count += 1;
            else
                break;

        }

        root.children.erase(root.children.begin() + count, root.children.end());

        // MIN @ L1: ////////////////////////////

        for (UINT i = 0; i < root.children.size(); i += 1) {

            auto & elem_l1 = root.children[i];

            int min_l1 = node_min_ext(elem_l1.node.get(), &root, player_index, &elem_l1.move, max_depth + 2, estimate_fn, use_pruning, true);

            //if (elem_l1.node->children.empty()) continue;

            std::sort(elem_l1.node->children.rbegin(), elem_l1.node->children.rend());

            UINT count = 0;
            for (auto & item : elem_l1.node->children) {

                if (std::abs(item.node->value - min_l1) <= 10) // Within 10 of min_l1
                    count += 1;
                else
                    break;

            }

            elem_l1.node->children.erase(elem_l1.node->children.begin() + count, elem_l1.node->children.end());

            // MAX @ L2: ////////////////////////

            for (UINT t = 0; t < elem_l1.node->children.size(); t += 1) {
            
                auto & elem_l2 = elem_l1.node->children[t];

                int max_l2 = node_max_ext(elem_l2.node.get(), elem_l1.node.get(), player_index, &elem_l2.move, max_depth + 4, estimate_fn, use_pruning, true);

                //if (elem_l2.node->children.empty()) continue;

                std::sort(elem_l2.node->children.rbegin(), elem_l2.node->children.rend());

                UINT count = 0;
                for (auto & item : elem_l2.node->children) {

                    if (std::abs(item.node->value - max_l2) <= 10) // Within 10 of max_l2
                        count += 1;
                    else
                        break;

                }

                elem_l2.node->children.erase(elem_l2.node->children.begin() + count, elem_l2.node->children.end());

                UINT pos = random_choice ? Random(count - 1) : 0;

                if (elem_l2.node->children[pos].node->value > best_val) {
                    best_val  = elem_l2.node->children[pos].node->value;
                    best_move = root.children[i].move;
                }

            }

            // END MAX //////////////////////////

        }

        // END MIN //////////////////////////////

        assert( best_val != util::min_val<int>() );

        return best_move;*/

    }

    ///////////////////////////////////////////////////////////////////////////

    namespace {
    
        std::string ReptSpace(UINT cnt) {
        
            std::string rv = "";

            for (UINT i = 0; i < cnt; i += 1) rv += " "; // Very unoptimized, can be done with resize and memset!

            return rv;

        }

    }

    /*void MinimaxTree::print_node(const Node * node) {

        for (auto & item : node->children) {
        
            std::cout << ReptSpace(item.node->depth * 2) + item.move.to_string() << "\n";

            print_node(item.node.get());

        }

    }*/

    /*void MinimaxTree::print() const {
    
        print_node(&root);

    }*/

    ///////////////////////////////////////////////////////////////////////////

    void MinimaxTree::Node::generate_children(UINT player_index) {

        children.clear();

        for (UINT pawn = 0; pawn < 2; pawn += 1) { // Loop through all pawns

            const Char2d orig_pos = state.get_pos(player_index, pawn);

            for (char xoff = -1; xoff <= 1; xoff += 1)
                for (char yoff = -1; yoff <= 1; yoff += 1) { // Loop through possible locations for the selected pawn

                    if (xoff == 0 && yoff == 0) continue;

                    const Char2d new_pos = Char2d{ orig_pos.x + xoff, orig_pos.y + yoff };

                    for (char bxoff = -1; bxoff <= 1; bxoff += 1)
                        for (char byoff = -1; byoff <= 1; byoff += 1) { // Loop through possible locations for building

                            if (bxoff == 0 && byoff == 0) continue;

                            const Char2d bld_pos = Char2d{ new_pos.x + bxoff, new_pos.y + byoff };

                            const GameMove move{ player_index, pawn, new_pos, bld_pos };

                            const GameState state_after_move = state.test_move(move);

                            if (state_after_move.is_valid()) { // If the move is valid, add it to the tree
                            
                                // CONSTRUCTOR REMINDER:
                                // Node(UINT depth = 0, const GameState & state = GameState{})
                                // Elem(const GameMove & move = GameMove{})
                                // Elem(std::unique_ptr<Node> ptr, const GameMove & move = GameMove{})

                                auto ptr = std::make_unique<Node>(depth + 1u, state_after_move);

                                children.emplace_back( std::move(ptr), move );

                            }

                        }

                }

        }
    
    } // End MinimaxTree::Node::generate_children(...)
 
    bool MinimaxTree::Node::test_victory(UINT just_played) const {

        // Standing on height 3:
        Char2d pos0 = state.get_pos(just_played, 0);
        Char2d pos1 = state.get_pos(just_played, 1);

        if (state.at(UINT(pos0.x), UINT(pos0.y)) == 3 || 
            state.at(UINT(pos1.x), UINT(pos1.y)) == 3) {

            return true;

        }

        // See if opponent can make a valid move:
        UINT opponent = ReversePlayerIndex(just_played);
        
        for (UINT pawn = 0; pawn < 2; pawn += 1) { // Loop through all pawns

            const Char2d orig_pos = state.get_pos(opponent, pawn);

            for (char xoff = -1; xoff <= 1; xoff += 1)
                for (char yoff = -1; yoff <= 1; yoff += 1) { // Loop through possible locations for the selected pawn

                    if (xoff == 0 && yoff == 0) continue;

                    const Char2d new_pos = Char2d{ orig_pos.x + xoff, orig_pos.y + yoff };

                    for (char bxoff = -1; bxoff <= 1; bxoff += 1)
                        for (char byoff = -1; byoff <= 1; byoff += 1) { // Loop through possible locations for building

                            if (bxoff == 0 && byoff == 0) continue;

                            const Char2d bld_pos = Char2d{ new_pos.x + bxoff, new_pos.y + byoff };

                            const GameMove move{ opponent, pawn, new_pos, bld_pos };

                            const GameState state_after_move = state.test_move(move);

                            if (state_after_move.is_valid()) { 
                                
                                // If the move is valid, the opponent CAN make a move, and the game is not yet won
                                return false;

                            }

                        }

                }

        }

        // Opponent CAN'T make a move:
        return true;

    } // End MinimaxTree::Node::test_victory(...)

}