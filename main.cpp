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

static std::string runFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

int main(int argc, const char** argv) {
    VM vm;
    /*size_t testSize = 73473457;

    std::cout << Memory::compressOpCodesToValue<size_t>(Memory::splitValueIntoOpCodes(testSize)) << "\n";

    std::cout << "\n" << testSize << "\n";*/
    /*

    Batch batch;

    //Add constant '1.2'
    uint16_t index = batch.addConstant(1.2);
    batch.addOp(Op::Code::Constant, 11);
    batch.addOps(Memory::toOpCodes(index), 11);

    //Add constant '345'
    size_t index2 = batch.addConstant(345);
    batch.addOp(Op::Code::Constant32, 12);
    batch.addOps(Memory::toOpCodes(index2), 12);

    //Negate stack top
    batch.addOp(Op::Code::Negate, 12);

    //Add
    batch.addOp(Op::Code::Add, 12);

    batch.addOp(Op::Code::Return, 12);
    Debug::disassembleBatch(batch);

    vm.interpret(batch);
     */

    if (argc == 1) {
        repl(vm);
    }
    else if (argc == 2) {
        runFile(argv[1]);
    }
    else {
        std::cout << "Usage: Titan [path]\n";
        return TOO_MANY_ARGS;
    }

    return 0;
}
