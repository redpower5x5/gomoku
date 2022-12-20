#ifndef INCLUDE_AI_NEGAMAX_H_
#define INCLUDE_AI_NEGAMAX_H_

#include <vector>

class GoBanAINegamax {
 public:
    GoBanAINegamax();
    ~GoBanAINegamax();

    static void heuristicNegamax(const char *gs, int player, int depth, int time_limit, bool enable_ab_pruning,
                                 int *actual_depth, int *move_r, int *move_c);

 private:
    // Preset search breadth
    // From root to leaf, each element is for 2 layers
    // e.g. {10, 5, 2} -> 10, 10, 5, 5, 2, 2, 2, ...
    static int presetSearchBreadth[5];

    // A move (candidate)
    struct Move {
        int r;
        int c;
        int heuristic_val;
        int actual_score;

        // Overloads < for sorting
        bool operator<(Move other) const {
            return heuristic_val > other.heuristic_val;
        }
    };

    static int heuristicNegamax(char *gs, int player, int initial_depth, int depth,
                                bool enable_ab_pruning, int alpha, int beta,
                                int *move_r, int *move_c);

    // Search possible moves based on a given state, sorted by heuristic values.
    static void searchMovesOrdered(const char *gs, int player, std::vector<Move> *result);

    // Currently not used
    static int negamax(char *gs, int player, int depth,
                       int *move_r, int *move_c);
};

#endif  // INCLUDE_AI_NEGAMAX_H_
