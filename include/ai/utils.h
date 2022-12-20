#ifndef INCLUDE_AI_UTILS_H_
#define INCLUDE_AI_UTILS_H_

#include <utils/globals.h>

class GoBanAIUtils {
 public:
    GoBanAIUtils();
    ~GoBanAIUtils();

    static inline char getCell(const char *gs, int r, int c) {
        if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return -1;
        return gs[g_board_size * r + c];
    }

    static inline bool setCell(char *gs, int r, int c, char value) {
        if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return false;
        gs[g_board_size * r + c] = value;
        return true;
    }

    static bool remoteCell(const char *gs, int r, int c);

    // Game state hashing
    static void zobristInit(int size, uint64_t *z1, uint64_t *z2);
    static uint64_t zobristHash(const char *gs, int size, uint64_t *z1, uint64_t *z2);
    static inline void zobristToggle(uint64_t *state, uint64_t *z1, uint64_t *z2,
                                     int row_size, int r, int c, int player) {
        if (player == 1) {
            *state ^= z1[row_size * r + c];
        } else if (player == 2) {
            *state ^= z2[row_size * r + c];
        }
    }

};

#endif  // INCLUDE_AI_UTILS_H_
