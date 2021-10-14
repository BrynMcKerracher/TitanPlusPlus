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
                std::cout << "[" << value.toString() << "]";
            }
            std::cout << "\n";
            Debug::disassembleInstruction(batch, (size_t)(pc - &batch.opcodes[0]));
        #endif //DEBUG_TRACE_EXECUTION
        //Op::Code dispatch
        switch (*pc++) {
            case Op::Code::Add: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                const auto operands = popBinaryOperands();
                stack.push_back(Value::fromNumber(operands[1].as.number + operands[0].as.number));
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
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                const auto operands = popBinaryOperands();
                stack.push_back(Value::fromNumber(operands[1].as.number / operands[0].as.number));
                break;
            }
            case Op::Code::Equal: {
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(b == a));
                break;
            }
            case Op::Code::False: {
                stack.push_back(Value::fromBool(false));
                break;
            }
            case Op::Code::Greater: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(a.as.number > b.as.number));
                break;
            }
            case Op::Code::GreaterEqual: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(a.as.number >= b.as.number));
                break;
            }
            case Op::Code::Less: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(a.as.number < b.as.number));
                break;
            }
            case Op::Code::LessEqual: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(a.as.number <= b.as.number));
                break;
            }
            case Op::Code::Multiply: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                const auto operands = popBinaryOperands();
                stack.push_back(Value::fromNumber(operands[1].as.number * operands[0].as.number));
                break;
            }
            case Op::Code::Negate: {
                if (stack.back().type != Value::Type::NUMBER) {
                    runtimeError("Operand must be a number.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                stack.back().as.number *= -1;
                break;
            }
            case Op::Code::Not: {
                stack.push_back(Value::fromBool(isFalsey(stack.back())));
                break;
            }
            case Op::Code::NotEqual: {
                Value b = popValue();
                Value a = popValue();
                stack.push_back(Value::fromBool(!(b == a)));
                break;
            }
            case Op::Code::Null: {
                stack.push_back(Value::fromNull());
                break;
            }
            case Op::Code::Return: {
                std::cout << stack.back().toString() << "\n";
                stack.pop_back();
                return InterpretResult::OK;
            }
            case Op::Code::Subtract: {
                if (stack.back().type != Value::Type::NUMBER || stack[stack.size() - 1].type != Value::Type::NUMBER) {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                const auto operands = popBinaryOperands();
                stack.push_back(Value::fromNumber(operands[1].as.number - operands[0].as.number));
                break;
            }
            case Op::Code::True: {
                stack.push_back(Value::fromBool(true));
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

void VM::runtimeError(const std::string &format, Batch& batch) {
    std::cout << format << "\n";
    size_t instruction = pc - &batch.opcodes[0] - 1;
    size_t line = batch.lines[instruction];
    std::cerr << "[Line " << line << "] in script\n";

    stack.clear();
}

bool VM::isFalsey(const Value &value) {
    return value.type == Value::Type::NIL || (value.type == Value::Type::BOOL && !value.as.boolean);
}
