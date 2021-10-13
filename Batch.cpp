//
// Created by Bryn McKerracher on 4/10/2021.
//

#include <iostream>
#include "Batch.h"

void Batch::addOp(Op::Code op, int lineNum) {
    std::cout << "Op " << Op::instructionName(op) << "\n";
    opcodes.push_back(op);
    lines.push_back(lineNum);
}

void Batch::addOps(const std::vector<Op::Code> &codes, int lineNum) {
    for (auto& code : codes) {
        addOp(code, lineNum);
    }
}

size_t Batch::addConstant(Value value) {
    constantPool.push_back(value);
    return constantPool.size() - 1;
}
