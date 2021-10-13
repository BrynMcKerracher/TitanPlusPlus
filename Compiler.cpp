#include "Compiler.h"

Compiler::Compiler() {
    parseRules[Token::Type::LEFT_PAREN]    = {[this] { grouping(); }, nullptr,   Precedence::NONE};
    parseRules[Token::Type::RIGHT_PAREN]   = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::LEFT_BRACE]    = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::RIGHT_BRACE]   = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::COMMA]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::DOT]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::MINUS]         = {[this] { unary(); },  [this] { binary(); },    Precedence::TERM};
    parseRules[Token::Type::PLUS]          = {nullptr,     [this] { binary(); },    Precedence::TERM};
    parseRules[Token::Type::SEMICOLON]     = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::SLASH]         = {nullptr,     [this] { binary(); },    Precedence::FACTOR};
    parseRules[Token::Type::STAR]          = {nullptr,     [this] { binary(); },    Precedence::FACTOR};
    parseRules[Token::Type::BANG]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::BANG_EQUAL]    = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::EQUAL]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::EQUAL_EQUAL]   = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::GREATER]       = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::GREATER_EQUAL] = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::LESS]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::LESS_EQUAL]    = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::IDENTIFIER]    = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::STRING]        = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::NUMBER]        = {[this] { number(); },      nullptr,   Precedence::NONE};
    parseRules[Token::Type::AND]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::CLASS]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::ELSE]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FALSE]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FOR]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FUN]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::IF]            = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::NIL]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::OR]            = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::PRINT]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::RETURN]        = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::SUPER]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::THIS]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::TRUE]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::VAR]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::WHILE]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::ERROR]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::END_OF_FILE]   = {nullptr,     nullptr,   Precedence::NONE};
}

bool Compiler::compile(const std::string &titanCode, Batch& batch) {
    titanSourceCode = titanCode;
    currentBatch = &batch;
    scanner.init(titanCode);

    advance();
    expression();
    consume(Token::Type::END_OF_FILE, "Expect end of expression.");
    emitOp(Op::Code::Return);
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        Debug::disassembleBatch(batch);
    }
#endif //DEBUG_PRINT_CODE
    return !parser.hadError;
}

Compiler::ParseRule *Compiler::getRule(Token::Type type) {
    return &parseRules[type];
}

void Compiler::advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanner.scanToken();

        if (parser.current.type != Token::Type::ERROR) break;

        error(parser.current, "Expect end of expression.");
    }
}

void Compiler::error(Token &token, const std::string &message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    std::cerr << "[Line " << token.line << "] Error";

    switch (token.type) {
        case Token::Type::END_OF_FILE: std::cerr << " at end";                                    break;
        case Token::Type::ERROR:                                                                   break;
        default: std::cerr << " at '" << titanSourceCode.substr(token.start, token.length) << "'"; break;
    }

    std::cerr << ": " << message << "\n";
    parser.hadError = true;
}

void Compiler::consume(Token::Type tokenType, const std::string &message) {
    if (parser.current.type == tokenType) {
        advance();
        return;
    }
    error(parser.current, message);
}

void Compiler::emitOp(Op::Code op) {
    currentBatch->addOp(op, parser.previous.line);
}

void Compiler::emitOps(Op::Code a, Op::Code b) {
    emitOp(a);
    emitOp(b);
}

void Compiler::expression() {
    parsePrecedence(Precedence::ASSIGNMENT);
}

void Compiler::number() {
    double value = strtod(&titanSourceCode[parser.previous.start], nullptr);
    emitConstant(value);
}

void Compiler::grouping() {
    expression();
    consume(Token::Type::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::unary() {
    Token::Type operatorType = parser.previous.type;

    parsePrecedence(Precedence::UNARY);

    switch (operatorType) {
        case Token::Type::MINUS: emitOp(Op::Code::Negate); break;
        default: return;
    }
}

void Compiler::binary() {
    Token::Type operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case Token::PLUS:          emitOp(Op::Code::Add);      break;
        case Token::MINUS:         emitOp(Op::Code::Subtract); break;
        case Token::STAR:          emitOp(Op::Code::Multiply); break;
        case Token::SLASH:         emitOp(Op::Code::Divide);   break;
        default: return; // Unreachable.
    }
}

void Compiler::parsePrecedence(Compiler::Precedence precedence) {
    advance();
    ParseFunction prefixRule = getRule(parser.previous.type)->prefix;//getRule(parser.previous.type)->prefix;
    if (prefixRule == nullptr) {
        error(parser.previous, "Expect expression.");
        return;
    }
    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFunction infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

void Compiler::emitConstant(Value value) {
    //Gets the current index in the constant pool.
    size_t constantIndex = currentBatch->addConstant(value);
    //If the current index won't fit in the address space of a single Op::Code, use a 32-bit address.
    if (constantIndex == (size_t)-1) {
        error(parser.previous, "Error adding new constant: Out of 32-bit address space.");
    }
    else if (constantIndex >= 1 << (sizeof(Op::Code) * 8)) {
        emitOp(Op::Code::Constant32);
        auto indexAsOpCodes = Memory::toOpCodes(constantIndex);
        emitOps(indexAsOpCodes[0], indexAsOpCodes[1]);
    }
    else {
        emitOp(Op::Code::Constant);
        emitOp((Op::Code)constantIndex);
    }
}




