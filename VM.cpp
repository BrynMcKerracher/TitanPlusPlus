#include "VM.h"

VM::VM() {
    stack.reserve(InitialStackReserveSize);
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
        //Print stack values and disassemble instructions if we're in debug mode.
        #ifdef DEBUG_TRACE_EXECUTION
            if (!stack.empty()) {
                std::cout << "\t\t";
                for (auto &value: stack) {
                    std::cout << "[" << value.toString() << "]";
                }
                std::cout << "\n";
            }
            Debug::disassembleInstruction(batch, (size_t) (pc - &batch.opcodes[0]));
        #endif //DEBUG_TRACE_EXECUTION
        //Op::Code dispatch
        switch (*pc++) {
            case Op::Code::Add: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromNumber(operands[1].toType<double>() + operands[0].toType<double>()));
                }
                else if (doBinaryOperandsHaveType(Value::Type::STRING)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromString(operands[1].toType<std::string>() + operands[0].toType<std::string>()));
                }
                else if (doBinaryOperandsHaveType(Value::Type::MATRIXF)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromMatrixF(operands[1].toType<MatrixF>() + operands[0].toType<MatrixF>()));
                }
                else if (doBinaryOperandsHaveType(Value::Type::MATRIXD)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromMatrixD(operands[1].toType<MatrixD>() + operands[0].toType<MatrixD>()));
                }
                else {
                    runtimeError("Operands must be two numbers, matrices, or strings.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::Constant64: {
                Value constant = batch.constantPool[read64()];
                stack.push_back(constant);
                break;
            }
            case Op::Code::Constant32: {
                Value constant = batch.constantPool[read32()];
                stack.push_back(constant);
                break;
            }
            case Op::Code::Constant: {
                Value constant = batch.constantPool[read16()];
                stack.push_back(constant);
                break;
            }
            case Op::Code::DefineGlobal64: {
                Value globalVarName = batch.constantPool[read64()];
                globals[globalVarName.toString()] = stack.back();
                stack.pop_back();
                stack.pop_back();
                break;
            }
            case Op::Code::DefineGlobal32: {
                Value globalVarName = batch.constantPool[read32()];
                globals[globalVarName.toString()] = stack.back();
                stack.pop_back();
                stack.pop_back();
                break;
            }
            case Op::Code::DefineGlobal: {
                Value globalVarName = batch.constantPool[read16()];
                globals[globalVarName.toString()] = stack.back();
                stack.pop_back();
                stack.pop_back();
                break;
            }
            case Op::Code::Divide: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromNumber(operands[1].toType<double>() / operands[0].toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
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
            case Op::Code::GetGlobal64: {
                Value globalVarName = batch.constantPool[read64()];
                stack.pop_back();
                try {
                    stack.push_back(globals.at(globalVarName.toString()));
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::GetGlobal32: {
                Value globalVarName = batch.constantPool[read32()];
                stack.pop_back();
                try {
                    stack.push_back(globals.at(globalVarName.toString()));
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::GetGlobal: {
                Value globalVarName = batch.constantPool[read16()];
                stack.pop_back();
                try {
                    stack.push_back(globals.at(globalVarName.toString()));
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::GetLocal64: {
                stack.push_back(stack[read64()]);
                break;
            }
            case Op::Code::GetLocal32: {
                stack.push_back(stack[read32()]);
                break;
            }
            case Op::Code::GetLocal: {
                stack.push_back(stack[read16()]);
                break;
            }
            case Op::Code::Greater: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    Value b = popValue();
                    Value a = popValue();
                    stack.push_back(Value::fromBool(a.toType<double>() > b.toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::GreaterEqual: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    Value b = popValue();
                    Value a = popValue();
                    stack.push_back(Value::fromBool(a.toType<double>() >= b.toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::Jump: {
                Op::Code offset = *pc++;
                pc += offset;
                break;
            }
            case Op::Code::JumpBack: {
                Op::Code offset = *pc++;
                pc -= offset;
                break;
            }
            case Op::Code::JumpIfFalse: {
                Op::Code offset = *pc++;
                if (isFalsey(stack.back())) pc += offset;
                break;
            }
            case Op::Code::JumpIfFalsePop: {
                Op::Code offset = *pc++;
                if (isFalsey(stack.back())) pc += offset;
                stack.pop_back();
                break;
            }
            case Op::Code::Less: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    Value b = popValue();
                    Value a = popValue();
                    stack.push_back(Value::fromBool(a.toType<double>() < b.toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::LessEqual: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    Value b = popValue();
                    Value a = popValue();
                    stack.push_back(Value::fromBool(a.toType<double>() <= b.toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::Multiply: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromNumber(operands[1].toType<double>() * operands[0].toType<double>()));
                }
                else {
                    runtimeError("Operands must be numbers.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                break;
            }
            case Op::Code::Negate: {
                if (stack.back().type != Value::Type::NUMBER) {
                    runtimeError("Operand must be a number.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                stack.back().data = stack.back().toType<double>() * -1;
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
            case Op::Code::Pop: {
                stack.pop_back();
                break;
            }
            case Op::Code::PopN: {
                stack.resize(stack.size() - *pc++);
                break;
            }
            case Op::Code::Print: {
                std::cout << stack.back().toString() << "\n";
                stack.pop_back();
                break;
            }
            case Op::Code::Return: {
                return InterpretResult::OK;
            }
            case Op::Code::SetGlobal64: {
                Value globalVarName = batch.constantPool[read64()];
                try {
                    globals.at(globalVarName.toString()) = stack.back();
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                stack.pop_back();
                break;
            }
            case Op::Code::SetGlobal32: {
                Value globalVarName = batch.constantPool[read32()];
                try {
                    globals.at(globalVarName.toString()) = stack.back();
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                stack.pop_back();
                break;
            }
            case Op::Code::SetGlobal: {
                Value globalVarName = batch.constantPool[read16()];
                try {
                    globals.at(globalVarName.toString()) = stack.back();
                }
                catch (const std::out_of_range& exception) {
                    runtimeError("Undefined variable '" + globalVarName.toString() + "'", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
                stack.pop_back();
                break;
            }
            case Op::Code::SetLocal64: {
                stack[read64()] = stack.back();
                break;
            }
            case Op::Code::SetLocal32: {
                stack[read32()] = stack.back();
                break;
            }
            case Op::Code::SetLocal: {
                stack[read16()] = stack.back();
                break;
            }
            case Op::Code::Subtract: {
                if (doBinaryOperandsHaveType(Value::Type::NUMBER)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromNumber(operands[1].toType<double>() - operands[0].toType<double>()));
                }
                else if (doBinaryOperandsHaveType(Value::Type::MATRIXF)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromMatrixF(operands[1].toType<MatrixF>() - operands[0].toType<MatrixF>()));
                }
                else if (doBinaryOperandsHaveType(Value::Type::MATRIXD)) {
                    const auto operands = popBinaryOperands();
                    stack.push_back(Value::fromMatrixD(operands[1].toType<MatrixD>() - operands[0].toType<MatrixD>()));
                }
                else {
                    runtimeError("Operands must be two numbers or two matrices.", batch);
                    return InterpretResult::RUNTIME_ERROR;
                }
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
    return value.type == Value::Type::NIL || (value.type == Value::Type::BOOL && !value.toType<bool>());
}

bool VM::doBinaryOperandsHaveType(Value::Type type) const {
    return stack.size() > 1 && stack.back().type == type && stack[stack.size() - 2].type == type;
}

uint16_t VM::read16() {
    uint16_t sum = *(uint16_t*)pc;
    move16();
    return sum;
}

uint32_t VM::read32() {
    uint32_t sum = *(uint32_t*)pc;
    move32();
    return sum;
}

uint64_t VM::read64() {
    uint64_t sum = *(uint64_t*)pc;
    move64();
    return sum;
}

void VM::move16() {
    pc += 2 / sizeof(Op::Code);
}

void VM::move32() {
    pc += 4 / sizeof(Op::Code);
}

void VM::move64() {
    pc += 8 / sizeof(Op::Code);
}