#include <api/renju_api.h>
#include <ai/ai_controller.h>
#include <ai/utils.h>
#include <utils/globals.h>
#include <cstring>

bool GoBanAPI::generateMove(const char *gs_string, int ai_player_id,
                            int search_depth, int time_limit, int num_threads,
                            int *actual_depth, int *move_r, int *move_c, int *winning_player,
                            unsigned int *node_count, unsigned int *eval_count, unsigned int *pm_count) {
    // Check input data
    if (strlen(gs_string) != g_gs_size ||
        ai_player_id  < 1 || ai_player_id > 2 ||
        search_depth == 0 || search_depth > 10 ||
        time_limit < 0    ||
        num_threads  < 1) {
        return false;
    }

    // Copy game state
    char *gs = new char[g_gs_size];
    std::memcpy(gs, gs_string, g_gs_size);

    // Convert from string
    gsFromString(gs_string, gs);

    // Generate move
    GoBanAIController::generateMove(gs, ai_player_id, search_depth, time_limit, actual_depth,
                                    move_r, move_c, winning_player, node_count, eval_count, pm_count);

    // Release memory
    delete[] gs;
    return true;
}

void GoBanAPI::gsFromString(const char *gs_string, char *gs) {
    if (strlen(gs_string) != g_gs_size) return;
    for (int i = 0; i < static_cast<int>(g_gs_size); i++) {
        gs[i] = gs_string[i] - '0';
    }
}

std::string GoBanAPI::renderGameState(const char *gs) {
    std::string result = "";
    for (int r = 0; r < g_board_size; r++) {
        for (int c = 0; c < g_board_size; c++) {
            result.push_back(GoBanAIUtils::getCell(gs, r, c) + '0');
            result.push_back(' ');
        }
        result.push_back('\n');
    }
    return result;
}
