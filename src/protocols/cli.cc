#include <protocols/cli.h>
#include <api/renju_api.h>
#include <utils/json.h>
#include <utils/globals.h>
#include <ctime>
#include <cstdlib>
#include <cstring>

bool GoBanProtocolCLI::beginSession(int argc, char const *argv[]) {
    // Print usage if no arguments provided
    if (argc < 2) {
        std::cerr << "Usage: renju" << std::endl;
        std::cerr << "        -s <state>       The game state (required)" << std::endl;
        std::cerr << "       [-p <ai_player>]  AI player (1: black, 2: white; default: 1)" << std::endl;
        std::cerr << "       [-d <depth>]      AI Search depth (iterative deepening)" << std::endl;
        std::cerr << "       [-l <time_limit>] Execution time limit for iterative deepening (5000)" << std::endl;
        std::cerr << "       [-t <threads>]    Number of threads (1)" << std::endl;
        return false;
    }

    // Initialize arguments
    g_board_size = 19;
    g_gs_size = (unsigned int)g_board_size * g_board_size;
    char gs_string[362] = {0};
    int ai_player = 1;
    int num_threads = 1;
    int search_depth = -1;
    int time_limit = 5500;

    // Iterate through arguments
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];

        if (strncmp(arg, "-s", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) continue;

            // Validate and copy state
            if (validateString(argv[i + 1], 361) == 361)
                memcpy(gs_string, argv[i + 1], 361);

        } else if (strncmp(arg, "-p", 2) == 0) {
            // AI player ID
            if (i >= argc - 1) continue;
            parseIntegerArgument(argv[i + 1], 3, &ai_player);

        } else if (strncmp(arg, "-d", 2) == 0) {
            // Search depth
            if (i >= argc - 1) continue;
            parseIntegerArgument(argv[i + 1], 3, &search_depth);

        } else if (strncmp(arg, "-l", 2) == 0) {
            // Number of threads
            if (i >= argc - 1) continue;
            parseIntegerArgument(argv[i + 1], 8, &time_limit);

        } else if (strncmp(arg, "-t", 2) == 0) {
            // Number of threads
            if (i >= argc - 1) continue;
            parseIntegerArgument(argv[i + 1], 3, &num_threads);

        } else if (strncmp(arg, "test", 4) == 0) {
            // Build test data
            memcpy(gs_string, "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002121000000000000001211112000000000000022122110000000000001211002200000000000002010200000000000000000200000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000", 362);
            search_depth = 8;
            ai_player = 2;
        }
    }

    std::string result = generateMove(gs_string, ai_player, search_depth, time_limit, num_threads);
    std::cout << result << std::endl;

    return true;
}

bool GoBanProtocolCLI::parseIntegerArgument(const char *str, int max_length, int *result) {
    if (validateString(str, max_length) < 0) return false;
    *result = (int)strtol(str, nullptr, 10);
    return true;
}

int GoBanProtocolCLI::validateString(const char *str, int max_length) {
    // Only supports up to 2048 bytes
    if (str == nullptr || max_length < 0 || max_length >= 2048) return -1;

    for (int i = 0; i <= max_length; i++) {
        if (str[i] == 0) return i;
    }
    return -1;
}

std::string GoBanProtocolCLI::generateMove(const char *gs_string, int ai_player_id, int search_depth,
                                           int time_limit, int num_threads) {
    // Record start time
    std::clock_t clock_begin = std::clock();

    // Generate move
    int move_r, move_c, winning_player, actual_depth;
    unsigned int node_count, eval_count, pm_count;
    bool success = GoBanAPI::generateMove(gs_string, ai_player_id, search_depth, time_limit, num_threads, &actual_depth,
                                          &move_r, &move_c, &winning_player, &node_count, &eval_count, &pm_count);

    if (!success) return generateResultJson(nullptr, "Invalid input data.");

    // Calculate elapsed CPU time
    std::clock_t clock_end = std::clock();
    std::clock_t cpu_time = (clock_end - clock_begin) * 1000 / CLOCKS_PER_SEC;

    // Build date & time
    std::string build_datetime = __DATE__;
    build_datetime = build_datetime + " " + __TIME__;

    // Generate result map
    std::unordered_map<std::string, std::string> data = {{"move_r", std::to_string(move_r)},
                                                         {"move_c", std::to_string(move_c)},
                                                         {"winning_player", std::to_string(winning_player)},
                                                         {"ai_player", std::to_string(ai_player_id)},
                                                         {"search_depth", std::to_string(actual_depth)},
                                                         {"cpu_time", std::to_string(cpu_time)},
                                                         {"num_threads", std::to_string(num_threads)},
                                                         {"node_count", std::to_string(node_count)},
                                                         {"eval_count", std::to_string(eval_count)},
                                                         {"pm_count", std::to_string(pm_count)},
                                                         {"cc_0", std::to_string(g_cc_0)},
                                                         {"cc_1", std::to_string(g_cc_1)},
                                                         {"build", build_datetime}};

    // Result
    return generateResultJson(&data, "ok");
}

std::string GoBanProtocolCLI::generateResultJson(const std::unordered_map<std::string, std::string> *data,
                                                 const std::string &message) {
    nlohmann::json result;
    if (data != nullptr) {
        // Add all k-v pairs to the result map
        for (auto pair : *data) {
            result["result"][pair.first] = pair.second;
        }
    } else {
        result["result"] = nullptr;
    }
    result["message"] = message;

    // Serialize
    return result.dump();
}
