#ifndef INCLUDE_PROTOCOLS_GOMOCUP_H_
#define INCLUDE_PROTOCOLS_GOMOCUP_H_

#include <string>

class GoBanProtocolGomocup {
 public:
    GoBanProtocolGomocup();
    ~GoBanProtocolGomocup();

    static bool beginSession(int argc, char const *argv[]);

 private:
    static void performAndWriteMove(char *gs_string, int time_limit);
    static void splitLine(const char *line, int *output);
    static void writeStdout(std::string str);
};

#endif  // INCLUDE_PROTOCOLS_GOMOCUP_H_
