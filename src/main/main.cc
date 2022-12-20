#include <protocols/cli.h>
#include <protocols/gomocup.h>
#include <cstring>

// Exclude main() if building with tests
#ifndef BLUPIG_TEST

int main(int argc, char const *argv[]) {
    if (argc <= 0) return 1;

    // Select Gomocup protocol if "pbrain' found in file name
    bool success;
    if (strstr(argv[0], "pbrain") != nullptr) {
        success = GoBanProtocolGomocup::beginSession(argc, argv);
    } else {
        success = GoBanProtocolCLI::beginSession(argc, argv);
    }
    return !success;
}

#endif  // BLUPIG_TEST
