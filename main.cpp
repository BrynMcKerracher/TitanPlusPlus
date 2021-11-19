#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Batch.h"
#include "Ops.h"
#include "Debug.h"
#include "Memory.h"
#include "VM.h"

enum ExitCodes {
    OK = 0,
    TOO_MANY_ARGS = -1,
    COMPILE_ERROR = -2,
    INTERPRET_ERROR = -3
};

static void repl(VM& vm) {
    std::string line;
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        std::cout << "\n";
        vm.interpret(line);
    }
}

static std::string fileToString(const std::string& path) {
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

int main(int argc, const char** argv) {
    VM vm;
    if (argc == 1) {
        repl(vm);
    }
    else if (argc == 2) {
        vm.interpret(fileToString(argv[1]));
    }
    else {
        std::cout << "Usage: Titan [path]\n";
        return TOO_MANY_ARGS;
    }
    return 0;
}
