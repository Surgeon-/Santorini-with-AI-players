#pragma once

#include "Utility.hpp"
#include "HelperStructs.hpp"
#include "EstimateFunc.hpp"

#include <memory>
#include <vector>
#include <string>

namespace etf_santorini_bj150087d {

    class MinimaxTree {
    
    public:

        static bool test_for_victory(const GameState & state, UINT just_played);
        static GameMove calc_next_move(const GameState & current_state, UINT player_index,
                                       UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool random_choice);
        static GameMove calc_next_move_ext(const GameState & current_state, UINT player_index,
                                           UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool random_choice);

    private:

        MinimaxTree() = delete;

        ///////////////////////////////////////////////////////////////////////
        struct Node {

            struct Elem {

                GameMove move;
                std::unique_ptr<Node> node;

                Elem(const GameMove & move = GameMove{})
                    : move(move), node(nullptr) { }

                Elem(std::unique_ptr<Node> ptr, const GameMove & move = GameMove{})
                    : move(move), node(nullptr) { node = std::move(ptr); }

                bool operator<(const Elem & other) {
                
                    return (node->value < other.node->value);

                }

            };

            UINT depth; // root->depth == 0
            GameState state;
            std::vector<Elem> children;
            int value, alpha, beta;

            Node(UINT depth = 0, const GameState & state = GameState{})
                : depth(depth)
                , state(state)
                , children()
                , value(0)
                , alpha(util::min_val<int>())
                , beta(util::max_val<int>())
                { }

            void set_state(const GameState & new_state) { state = new_state; }

            void generate_children(UINT player_index);

            void collapse() { children.clear(); }

            bool test_victory(UINT just_played) const; // arg0 = Player who just finished his move and
                                                       // brought the game to its current state.

        };
        ///////////////////////////////////////////////////////////////////////

        // Copy:
        MinimaxTree(const MinimaxTree& other) = delete;
        MinimaxTree& operator=(const MinimaxTree & other) = delete;

        // Move:
        MinimaxTree(MinimaxTree&& other) = delete;
        MinimaxTree& operator=(MinimaxTree && other) = delete;

        static int node_min(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse);
        static int node_max(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse);

        static int node_min_ext(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse);
        static int node_max_ext(Node * node, const Node * parent, UINT player_index, const GameMove * used_move, UINT max_depth, EstimateFn estimate_fn, bool use_pruning, bool no_collapse);

        static void print_node(const Node * node);

    };

}