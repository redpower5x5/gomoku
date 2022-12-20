#ifndef INCLUDE_AI_EVAL_H_
#define INCLUDE_AI_EVAL_H_

#define kRenjuAiEvalWinningScore 10000
#define kRenjuAiEvalThreateningScore 300

class GoBanAIEval {
 public:
    GoBanAIEval();
    ~GoBanAIEval();

    // Evaluate the entire game state as a player
    static int evalState(const char *gs, int player);

    // Evaluate one possible move as a player
    static int evalMove(const char *gs, int r, int c, int player);

    // Check if any player is winning based on a given state
    static int winningPlayer(const char *gs);

// Allow testing private members in this class
#ifndef BLUPIG_TEST
 private:
#endif
    // Result of a single direction measurement
    struct DirectionMeasurement {
        char length;          // Number of pieces in a row
        char block_count;     // Number of ends blocked by edge or the other player (0-2)
        char space_count;     // Number of spaces in the middle of pattern
    };

    // A single direction pattern
    struct DirectionPattern {
        char min_occurrence;  // Minimum number of occurrences to match
        char length;          // Length of pattern (pieces in a row)
        char block_count;     // Number of ends blocked by edge or the other player (0-2)
        char space_count;     // Number of spaces in the middle of pattern (-1: Ignore value)
    };

    // An array of preset patterns
    static DirectionPattern *preset_patterns;

    // Preset scores of each preset pattern
    static int *preset_scores;

    // Loads preset patterns into memory
    // preset_patterns_skip is the number of patterns to skip for a maximum
    // measured length in an all_direction_measurement (e.g. longest is 3 pieces
    // in an ADM, then skip first few patterns that require 4 pieces or more).
    static void generatePresetPatterns(DirectionPattern **preset_patterns,
                                       int **preset_scores,
                                       int *preset_patterns_size,
                                       int *preset_patterns_skip);

    // Evaluates an all-direction measurement
    static int evalADM(DirectionMeasurement *all_direction_measurement);

    // Tries to match a set of patterns with an all-direction measurement
    static int matchPattern(DirectionMeasurement *all_direction_measurement,
                            DirectionPattern *patterns);

    // Measures all 4 directions
    static void measureAllDirections(const char *gs,
                                     int r,
                                     int c,
                                     int player,
                                     bool consecutive,
                                     GoBanAIEval::DirectionMeasurement *adm);

    // Measure a single direction
    static void measureDirection(const char *gs,
                                 int r, int c,
                                 int dr, int dc,
                                 int player,
                                 bool consecutive,
                                 GoBanAIEval::DirectionMeasurement *result);
};

#endif  // INCLUDE_AI_EVAL_H_
