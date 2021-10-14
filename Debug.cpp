#include "Debug.h"

void Debug::disassembleBatch(const Batch &batch) {
    std::cout << "== ByteCode Disassembly ==\n";
    for (size_t instructionIndex = 0; instructionIndex < batch.opcodes.size();) {
        instructionIndex += disassembleInstruction(batch, instructionIndex);
    }
}

int Debug::disassembleInstruction(const Batch &batch, size_t instructionIndex) {
    //OpCode index
    std::cout << instructionIndex << "\t";

    //Print piping if on the same line, otherwise print the line number
    if (instructionIndex > 0 && batch.lines[instructionIndex] == batch.lines[instructionIndex - 1]) {
        std::cout << "| ";
    }
    else {
        std::cout << batch.lines[instructionIndex] << " ";
    }

    //OpCode name
    Op::Code instructionOpCode = batch.opcodes[instructionIndex];
    std::cout << Op::instructionName(instructionOpCode) << "\t";

    //OpCode-specific behaviour
    switch (instructionOpCode) {
        case Op::Constant32: {
            size_t constantIndex = Memory::toValue<size_t>(batch.opcodes[instructionIndex + 1], batch.opcodes[instructionIndex + 2]);
            std::cout << constantIndex << " " << batch.constantPool[constantIndex].toString();
            break;
        }
        case Op::Constant: {
            size_t constantIndex = batch.opcodes[instructionIndex + 1];
            std::cout << constantIndex << " " << batch.constantPool[constantIndex].toString();
            break;
        }
        default: break;
    }
    std::cout << "\n";

    return Op::instructionLength(instructionOpCode);
}