#include "Ops.h"

int Op::instructionLength(Op::Code op) {
    switch (op) {
        case Add:            return 1;
        case Constant32:     return 3;
        case Constant:       return 2;
        case DefineGlobal32: return 3;
        case DefineGlobal:   return 2;
        case Divide:         return 1;
        case Equal:          return 1;
        case False:          return 1;
        case GetGlobal32:    return 3;
        case GetGlobal:      return 2;
        case Greater:        return 1;
        case GreaterEqual:   return 1;
        case Less:           return 1;
        case LessEqual:      return 1;
        case Multiply:       return 1;
        case Negate:         return 1;
        case Not:            return 1;
        case NotEqual:       return 1;
        case Null:           return 1;
        case Pop:            return 1;
        case Print:          return 1;
        case Return:         return 1;
        case Subtract:       return 1;
        case True:           return 1;
        default:             return 1;
    }
}

std::string Op::instructionName(Op::Code op) {
    switch (op) {
        case Add:             return "OP_ADD";
        case Constant32:      return "OP_CONSTANT_32";
        case Constant:        return "OP_CONSTANT";
        case DefineGlobal32:  return "OP_DEFINE_GLOBAL_32";
        case DefineGlobal:    return "OP_DEFINE_GLOBAL";
        case Divide:          return "OP_DIVIDE";
        case Equal:           return "OP_EQUAL";
        case False:           return "OP_FALSE";
        case GetGlobal32:     return "OP_GET_GLOBAL_32";
        case GetGlobal:       return "OP_GET_GLOBAL";
        case Greater:         return "OP_GREATER";
        case GreaterEqual:    return "OP_GREATER_EQUAL";
        case Less:            return "OP_LESS";
        case LessEqual:       return "OP_LESS_EQUAL";
        case Multiply:        return "OP_MULTIPLY";
        case Negate:          return "OP_NEGATE";
        case Not:             return "OP_NOT";
        case NotEqual:        return "OP_NOT_EQUAL";
        case Null:            return "OP_NULL";
        case Pop:             return "OP_POP";
        case Print:           return "OP_PRINT";
        case Return:          return "OP_RETURN";
        case Subtract:        return "OP_SUBTRACT";
        case True:            return "OP_TRUE";
        default:              return "Unknown Op: " + std::to_string(op);
    }
}

