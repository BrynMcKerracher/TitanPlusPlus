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
        case GetLocal32:     return 3;
        case GetLocal:       return 2;
        case Greater:        return 1;
        case GreaterEqual:   return 1;
        case JumpIfFalse:    return 2;
        case Jump:           return 2;
        case Less:           return 1;
        case LessEqual:      return 1;
        case Multiply:       return 1;
        case Negate:         return 1;
        case Not:            return 1;
        case NotEqual:       return 1;
        case Null:           return 1;
        case Pop:            return 1;
        case PopN:           return 2;
        case Print:          return 1;
        case Return:         return 1;
        case SetGlobal32:    return 3;
        case SetGlobal:      return 2;
        case SetLocal32:     return 3;
        case SetLocal:       return 2;
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
        case GetLocal32:      return "OP_GET_LOCAL_32";
        case GetLocal:        return "OP_GET_LOCAL";
        case Greater:         return "OP_GREATER";
        case GreaterEqual:    return "OP_GREATER_EQUAL";
        case JumpIfFalse:     return "OP_JUMP_IF_FALSE";
        case Jump:            return "OP_JUMP";
        case Less:            return "OP_LESS";
        case LessEqual:       return "OP_LESS_EQUAL";
        case Multiply:        return "OP_MULTIPLY";
        case Negate:          return "OP_NEGATE";
        case Not:             return "OP_NOT";
        case NotEqual:        return "OP_NOT_EQUAL";
        case Null:            return "OP_NULL";
        case Pop:             return "OP_POP";
        case PopN:            return "OP_POP_N";
        case Print:           return "OP_PRINT";
        case Return:          return "OP_RETURN";
        case SetGlobal32:     return "OP_SET_GLOBAL_32";
        case SetGlobal:       return "OP_SET_GLOBAL";
        case SetLocal32:      return "OP_SET_LOCAL_32";
        case SetLocal:        return "OP_SET_LOCAL";
        case Subtract:        return "OP_SUBTRACT";
        case True:            return "OP_TRUE";
        default:              return "Unknown Op: " + std::to_string(op);
    }
}

