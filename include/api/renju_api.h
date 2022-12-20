#ifndef INCLUDE_API_RENJU_API_H_
#define INCLUDE_API_RENJU_API_H_

#include <string>

class GoBanAPI {
 public:
    GoBanAPI();
    ~GoBanAPI();

    // Generate move based on a given game state
    static bool generateMove(const char *gs_string, int ai_player_id,
                             int search_depth, int time_limit, int num_threads,
                             int *actual_depth, int *move_r, int *move_c, int *winning_player,
                             unsigned int *node_count, unsigned int *eval_count, unsigned int *pm_count);

    // Convert a game state string to game state binary array
    static void gsFromString(const char *gs_string, char *gs);

 private:
    // Render game state into text
    static std::string renderGameState(const char *gs);
};

#endif  // INCLUDE_API_RENJU_API_H_
