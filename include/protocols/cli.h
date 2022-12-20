#ifndef INCLUDE_PROTOCOLS_CLI_H_
#define INCLUDE_PROTOCOLS_CLI_H_

#include <string>
#include <unordered_map>

class GoBanProtocolCLI {
 public:
    GoBanProtocolCLI();
    ~GoBanProtocolCLI();

    static bool beginSession(int argc, char const *argv[]);

    // Generate move and responds in json
    static std::string generateMove(const char *gs_string, int ai_player_id, int search_depth,
                                    int time_limit, int num_threads);

 private:
    // Validates a string and parses into an integer
    static bool parseIntegerArgument(const char *str, int max_length, int *result);

    // Validates a string and returns the length.
    // If fails validation, -1 is returned.
    static int validateString(const char *str, int max_length);

    // Generate json response
    static std::string generateResultJson(const std::unordered_map<std::string, std::string> *data,
                                          const std::string &message);
};

#endif  // INCLUDE_PROTOCOLS_CLI_H_
