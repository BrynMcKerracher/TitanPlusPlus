#include "Ops.h"

int Op::instructionLength(Op::Code op) {
    switch (op) {
        case Add:           return 1;
        case Constant32:    return 3;
        case Constant:      return 2;
        case Divide:        return 1;
        case Multiply:      return 1;
        case Negate:        return 1;
        case Return:        return 1;
        case Subtract:      return 1;
        default:            return 1;
    }
}

std::string Op::instructionName(Op::Code op) {
    switch (op) {
        case Add:           return "OP_ADD";
        case Constant32:    return "OP_CONSTANT_32";
        case Constant:      return "OP_CONSTANT";
        case Divide:        return "OP_DIVIDE";
        case Multiply:      return "OP_MULTIPLY";
        case Negate:        return "OP_NEGATE";
        case Return:        return "OP_RETURN";
        case Subtract:      return "OP_SUBTRACT";
        default:            return "Unknown Op: " + std::to_string(op);
    }
}

