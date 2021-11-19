#include "Ops.h"

int Op::instructionLength(Op::Code op) {
    switch (op) {
        case Add:            return 1;
        case Constant64:     return 5;
        case Constant32:     return 3;
        case Constant:       return 2;
        case DefineGlobal64: return 5;
        case DefineGlobal32: return 3;
        case DefineGlobal:   return 2;
        case Divide:         return 1;
        case Equal:          return 1;
        case False:          return 1;
        case GetGlobal64:    return 5;
        case GetGlobal32:    return 3;
        case GetGlobal:      return 2;
        case GetLocal64:     return 5;
        case GetLocal32:     return 3;
        case GetLocal:       return 2;
        case Greater:        return 1;
        case GreaterEqual:   return 1;
        case Jump:           return 2;
        case JumpBack:       return 2;
        case JumpIfFalse:    return 2;
        case JumpIfFalsePop: return 2;
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
        case SetGlobal64:    return 5;
        case SetGlobal32:    return 3;
        case SetGlobal:      return 2;
        case SetLocal64:     return 5;
        case SetLocal32:     return 3;
        case SetLocal:       return 2;
        case Subtract:       return 1;
        case True:           return 1;
        default:             return 1;
    }
}

std::string Op::instructionName(Op::Code op) {
    switch (op) {
        case Add:             return "ADD";
        case Constant64:      return "CONSTANT_64";
        case Constant32:      return "CONSTANT_32";
        case Constant:        return "CONSTANT";
        case DefineGlobal64:  return "DEFINE_GLOBAL_64";
        case DefineGlobal32:  return "DEFINE_GLOBAL_32";
        case DefineGlobal:    return "DEFINE_GLOBAL";
        case Divide:          return "DIVIDE";
        case Equal:           return "EQUAL";
        case False:           return "FALSE";
        case GetGlobal64:     return "GET_GLOBAL_64";
        case GetGlobal32:     return "GET_GLOBAL_32";
        case GetGlobal:       return "GET_GLOBAL";
        case GetLocal64:      return "GET_LOCAL_64";
        case GetLocal32:      return "GET_LOCAL_32";
        case GetLocal:        return "GET_LOCAL";
        case Greater:         return "GREATER";
        case GreaterEqual:    return "GREATER_EQUAL";
        case Jump:            return "JUMP";
        case JumpBack:        return "JUMP_BACK";
        case JumpIfFalse:     return "JUMP_IF_FALSE";
        case JumpIfFalsePop:  return "JUMP_IF_FALSE_POP";
        case Less:            return "LESS";
        case LessEqual:       return "LESS_EQUAL";
        case Multiply:        return "MULTIPLY";
        case Negate:          return "NEGATE";
        case Not:             return "NOT";
        case NotEqual:        return "NOT_EQUAL";
        case Null:            return "NULL";
        case Pop:             return "POP";
        case PopN:            return "POP_N";
        case Print:           return "PRINT";
        case Return:          return "RETURN";
        case SetGlobal64:     return "SET_GLOBAL_64";
        case SetGlobal32:     return "SET_GLOBAL_32";
        case SetGlobal:       return "SET_GLOBAL";
        case SetLocal64:      return "SET_LOCAL_64";
        case SetLocal32:      return "SET_LOCAL_32";
        case SetLocal:        return "SET_LOCAL";
        case Subtract:        return "SUBTRACT";
        case True:            return "TRUE";
        default:              return "Unknown Op: " + std::to_string(op);
    }
}

