#include <protocols/gomocup.h>
#include <api/renju_api.h>
#include <utils/globals.h>
#include <cstring>
#include <iostream>

bool GoBanProtocolGomocup::beginSession(int argc, char const *argv[]) {
    char line[256];
    char *gs_string = nullptr;
    bool errored = false;
    int time_limit = 1500;

    while (std::cin.getline(line, 256)) {
        // Commands
        if (strncmp(line, "START", 5) == 0) {
            // START
            unsigned int board_size = (unsigned int)atoi(&line[6]);
            if (board_size >= 15 && board_size <= 20) {
                g_board_size = board_size;
                g_gs_size = (unsigned int)g_board_size * g_board_size;

                // Initialize game state
                gs_string = new char[g_gs_size + 1];
                memset(gs_string, 0, g_gs_size + 1);
                memset(gs_string, '0', g_gs_size);

                // Write output
                writeStdout("OK");
            } else {
                writeStdout("ERROR");
                errored = true;
                break;
            }
        } else if (strncmp(line, "END", 3) == 0) {
            // END
            break;
        } else if (strncmp(line, "BEGIN", 5) == 0) {
            // BEGIN [SIZE]
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Reset board
            memset(gs_string, '0', g_gs_size);

            // Put a piece in center
            int move_r = g_board_size / 2, move_c = g_board_size / 2;
            gs_string[g_board_size * move_r + move_c] = '1';

            // Write output
            std::cout << move_c << "," << move_r << std::endl;

        } else if (strncmp(line, "BOARD", 5) == 0) {
            // BOARD
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Reset board
            memset(gs_string, '0', g_gs_size);

            while (std::cin.getline(line, 256)) {
                // [X],[Y],[field]
                if (strncmp(line, "DONE", 4) == 0) {
                    break;
                } else {
                    // Read piece
                    int values[3] = {-1, -1, -1};
                    splitLine(line, values);

                    if (values[2] == -1) {
                        writeStdout("ERROR");
                        errored = true;
                        break;
                    }

                    // Update board
                    gs_string[g_board_size * values[1] + values[0]] = '0' + static_cast<char>(values[2]);
                }
            }

            // Generate, perform a move and write to stdout
            performAndWriteMove(gs_string, time_limit);

        } else if (strncmp(line, "TURN", 4) == 0) {
            // TURN [X],[Y]
            // Check board status
            if (gs_string == nullptr) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Read move
            int values[2] = {-1, -1};
            int move_r, move_c;
            splitLine(&line[5], values);
            move_c = values[0]; move_r = values[1];

            if (move_r == -1 || move_r >= g_board_size || move_c >= g_board_size) {
                writeStdout("ERROR");
                errored = true;
                break;
            }

            // Update board
            gs_string[g_board_size * move_r + move_c] = '2';

            // Generate, perform a move and write to stdout
            performAndWriteMove(gs_string, time_limit);

        } else if (strncmp(line, "INFO", 4) == 0) {
            // INFO [key] [value]
            if (strncmp(line + 5, "timeout_turn", 12) == 0) {
                time_limit = atoi(line + 5 + 12 + 1) + 500;
            }
        } else if (strncmp(line, "ABOUT", 5) == 0) {
            std::string build_datetime = __DATE__;
            build_datetime = build_datetime + " " + __TIME__;

            writeStdout("name=\"blupig\", version=\"" + build_datetime + "\", author=\"Yunzhu Li\", country=\"China\"");
        } else {
            writeStdout("UNKNOWN");
        }
    }

    // Release memory
    if (gs_string != nullptr) delete[] gs_string;

    return !errored;
}

void GoBanProtocolGomocup::performAndWriteMove(char *gs_string, int time_limit) {
    // Generate move
    int move_r, move_c, winning_player, actual_depth;
    unsigned int node_count, eval_count;
    bool success = GoBanAPI::generateMove(gs_string, 1, -1, time_limit, 1, &actual_depth, &move_r, &move_c,
                                          &winning_player, &node_count, &eval_count, nullptr);

    if (success) {
        // Write MESSAGE
        std::cout << "MESSAGE" <<
                     " d=" << actual_depth <<
                     " node_cnt=" << node_count <<
                     " eval_cnt=" << eval_count << std::endl;

        // Update board
        gs_string[g_board_size * move_r + move_c] = '1';

        // Write output
        std::cout << move_c << "," << move_r << std::endl;
    } else {
        writeStdout("ERROR");
    }
}

void GoBanProtocolGomocup::splitLine(const char *line, int *output) {
    // Copy input
    size_t in_length = strlen(line);
    char *_line = new char[in_length];
    memcpy(_line, line, in_length);

    int pos = 0, seg_idx = 0, seg_begin = 0;

    while (_line[pos] != 0) {
        if (_line[pos] == ',') {
            _line[pos] = 0;
            output[seg_idx++] = atoi(&_line[seg_begin]);
            seg_begin = pos + 1;
        }
        ++pos;
    }

    // Last one
    output[seg_idx] = atoi(&line[seg_begin]);

    delete[] _line;
}

void GoBanProtocolGomocup::writeStdout(std::string str) {
    std::cout << str << std::endl;
}
