#include "VM.h"

VM::VM() {
    stack.reserve(MaxStackSize);
}

VM::InterpretResult VM::interpret(const std::string &titanCode) {
    Compiler compiler;
    Batch batch;

    if (!compiler.compile(titanCode, batch)) {
        return InterpretResult::COMPILE_ERROR;
    }

    //Point PC to first instruction
    pc = &batch.opcodes[0];

    InterpretResult result = run(batch);

    return result;
}

VM::InterpretResult VM::run(Batch &batch) {
    while (true) {
        //Print stack values and disassemble instruction if we're in debug mode.
        #ifdef DEBUG_TRACE_EXECUTION
            std::cout << "\t\t";
            for (auto& value : stack) {
                std::cout << "[" << value << "]";
            }
            std::cout << "\n";
            Debug::disassembleInstruction(batch, (size_t)(pc - &batch.opcodes[0]));
        #endif //DEBUG_TRACE_EXECUTION
        //Op::Code dispatch
        switch (*pc++) {
            case Op::Code::Add: {
                const auto operands = popBinaryOperands();
                stack.push_back(operands[1] + operands[0]);
                break;
            }
            case Op::Code::Constant32: {
                Op::Code firstHalf = *pc++;
                Op::Code secondHalf = *pc++;
                Value constant = batch.constantPool[Memory::toValue<size_t>(firstHalf, secondHalf)];
                stack.push_back(constant);
                break;
            }
            case Op::Code::Constant: {
                Value constant = batch.constantPool[*pc++];
                stack.push_back(constant);
                break;
            }
            case Op::Code::Divide: {
                const auto operands = popBinaryOperands();
                stack.push_back(operands[1] / operands[0]);
                break;
            }
            case Op::Code::Multiply: {
                const auto operands = popBinaryOperands();
                stack.push_back(operands[1] * operands[0]);
                break;
            }
            case Op::Code::Negate: {
                stack.back() *= -1;
                break;
            }
            case Op::Code::Return: {
                std::cout << stack.back() << "\n";
                stack.pop_back();
                return InterpretResult::OK;
            }
            case Op::Code::Subtract: {
                const auto operands = popBinaryOperands();
                stack.push_back(operands[1] - operands[0]);
                break;
            }
        }
    }
    return VM::OK;
}

std::vector<Value> VM::popBinaryOperands() {
    return std::vector<Value>{popValue(), popValue()};
}

Value VM::popValue() {
    Value value = stack.back();
    stack.pop_back();
    return value;
}
