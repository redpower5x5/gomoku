#include <ai/utils.h>
#include <random>

bool GoBanAIUtils::remoteCell(const char *gs, int r, int c) {
    if (gs == nullptr) return false;
    for (int i = r - 2; i <= r + 2; ++i) {
        if (i < 0 || i >= g_board_size) continue;
        for (int j = c - 2; j <= c + 2; ++j) {
            if (j < 0 || j >= g_board_size) continue;
            if (gs[g_board_size * i + j] > 0) return false;
        }
    }
    return true;
}

void GoBanAIUtils::zobristInit(int size, uint64_t *z1, uint64_t *z2) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> d(0, UINT64_MAX);

    // Generate random values
    for (int i = 0; i < size; i++) {
        z1[i] = d(gen);
        z2[i] = d(gen);
    }
}

uint64_t GoBanAIUtils::zobristHash(const char *gs, int size, uint64_t *z1, uint64_t *z2) {
    uint64_t state = 0;
    for (int i = 0; i < size; i++) {
        if (gs[i] == 1) {
            state ^= z1[i];
        } else if (gs[i] == 2) {
            state ^= z2[i];
        }
    }
    return state;
}
