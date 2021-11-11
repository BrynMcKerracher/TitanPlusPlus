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
    parseRules[Token::Type::BANG]          = {[this] { unary(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::BANG_EQUAL]    = {nullptr,     [this] { binary(); },   Precedence::EQUALITY};
    parseRules[Token::Type::EQUAL]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::EQUAL_EQUAL]   = {nullptr,     [this] { binary(); },   Precedence::EQUALITY};
    parseRules[Token::Type::GREATER]       = {nullptr,     [this] { binary(); },   Precedence::COMPARISON};
    parseRules[Token::Type::GREATER_EQUAL] = {nullptr,     [this] { binary(); },   Precedence::COMPARISON};
    parseRules[Token::Type::LESS]          = {nullptr,     [this] { binary(); },   Precedence::COMPARISON};
    parseRules[Token::Type::LESS_EQUAL]    = {nullptr,     [this] { binary(); },   Precedence::COMPARISON};
    parseRules[Token::Type::IDENTIFIER]    = {[this] { variable(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::STRING]        = {[this] { string(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::NUMBER]        = {[this] { number(); },      nullptr,   Precedence::NONE};
    parseRules[Token::Type::MATRIX]        = {[this] { matrix(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::AND]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::CLASS]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::ELSE]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FALSE]         = {[this] { literal(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FOR]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::FUN]           = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::IF]            = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::NIL]           = {[this] { literal(); },     nullptr,   Precedence::NONE};
    parseRules[Token::Type::OR]            = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::PRINT]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::RETURN]        = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::SUPER]         = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::THIS]          = {nullptr,     nullptr,   Precedence::NONE};
    parseRules[Token::Type::TRUE]          = {[this] { literal(); },     nullptr,   Precedence::NONE};
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

    while (!matchType(Token::Type::END_OF_FILE)) {
        declaration();
    }

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
        case Token::Type::END_OF_FILE: std::cerr << " at end";                                     break;
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

Op::Code Compiler::emitJump(Op::Code jumpOp) {
    emitOp(jumpOp);
    emitOp((Op::Code)0);
    return (Op::Code)(currentBatch->opcodes.size() - 1);
}

void Compiler::backPatchJump(Op::Code offset) {
    if (currentBatch->opcodes.size() - offset - 1 >= (Op::Code)-1) {
        error(parser.current, "Jump back-patch overflow: Operand is too large for a single jump instruction.");
    }
    Op::Code jump = (Op::Code)(currentBatch->opcodes.size() - offset - 1);

    currentBatch->opcodes[offset] = jump;
}

void Compiler::expression() {
    parsePrecedence(Precedence::ASSIGNMENT);
}

void Compiler::block() {
    while (parser.current.type != Token::Type::RIGHT_BRACE and parser.current.type != Token::Type::END_OF_FILE) {
        declaration();
    }
    consume(Token::Type::RIGHT_BRACE, "Expect '}' after block.");
}

void Compiler::number() {
    double value = strtod(&titanSourceCode[parser.previous.start], nullptr);
    emitConstant(Value::fromNumber(value));
}

void Compiler::grouping() {
    expression();
    consume(Token::Type::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::unary() {
    Token::Type operatorType = parser.previous.type;

    parsePrecedence(Precedence::UNARY);

    switch (operatorType) {
        case Token::Type::BANG:  emitOp(Op::Code::Not);    break;
        case Token::Type::MINUS: emitOp(Op::Code::Negate); break;
        default: return;
    }
}

void Compiler::binary() {
    Token::Type operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case Token::BANG_EQUAL:    emitOp(Op::Code::NotEqual);     break;
        case Token::EQUAL_EQUAL:   emitOp(Op::Code::Equal);        break;
        case Token::GREATER:       emitOp(Op::Code::Greater);      break;
        case Token::GREATER_EQUAL: emitOp(Op::Code::GreaterEqual); break;
        case Token::LESS:          emitOp(Op::Code::Less);         break;
        case Token::LESS_EQUAL:    emitOp(Op::Code::LessEqual);    break;
        case Token::PLUS:          emitOp(Op::Code::Add);          break;
        case Token::MINUS:         emitOp(Op::Code::Subtract);     break;
        case Token::STAR:          emitOp(Op::Code::Multiply);     break;
        case Token::SLASH:         emitOp(Op::Code::Divide);       break;
        default: return; // Unreachable.
    }
}

void Compiler::literal() {
    switch (parser.previous.type) {
        case Token::FALSE: emitOp(Op::Code::False); break;
        case Token::NIL:   emitOp(Op::Code::Null);  break;
        case Token::TRUE:  emitOp(Op::Code::True);  break;
        default: break;
    }
}

void Compiler::string() {
    emitConstant(Value::fromString(titanSourceCode.substr(parser.previous.start + 1, parser.previous.length - 2)));
}

void Compiler::matrix() {
   // std::cout << "Matrix: '" << titanSourceCode.substr(parser.previous.start + 1, parser.previous.length - 1) << "'\n";
    emitConstant(Value::fromMatrixD(MatrixD(titanSourceCode.substr(parser.previous.start + 1, parser.previous.length - 2))));
}

void Compiler::declaration() {
    if (matchType(Token::Type::VAR)) {
        variableDeclaration();
    }
    else {
        statement();
    }

    if (parser.panicMode) {
        synchronise();
    }
}

void Compiler::statement() {
    if (matchType(Token::Type::PRINT)) {
        expression();
        consume(Token::Type::SEMICOLON, "Expect ';' after value.");
        emitOp(Op::Print);
    }
    else if (matchType(Token::Type::IF)) {
        ifStatement();
    }
    else if (matchType(Token::Type::LEFT_BRACE)) {
        beginScope();
        block();
        endScope();
    }
    else {
        expressionStatement();
    }
}

void Compiler::expressionStatement() {
    expression();
    consume(Token::Type::SEMICOLON, "Expect ';' after expression.");
    emitOp(Op::Code::Pop);
}

void Compiler::ifStatement() {
    //Parse 'if' statement
    consume(Token::Type::LEFT_PAREN, "Expect '(' after 'if'.");
    expression();
    consume(Token::Type::RIGHT_PAREN, "Expect ')' after condition.");

    Op::Code ifJump = emitJump(Op::Code::JumpIfFalse);
    emitOp(Op::Code::Pop);
    statement();

    //Op::Code elseJump = emitJump(Op::Code::Jump);

    backPatchJump(ifJump);
    emitOp(Op::Code::Pop);

    //Look for 'else'
   // if (matchType(Token::Type::ELSE)) {
   //     statement();
   // }
  //  backPatchJump(elseJump);
}

void Compiler::variableDeclaration() {
    size_t globalNameIndex = parseVariable("Expect variable name.");

    if (matchType(Token::Type::EQUAL)) {
        expression();
    }
    else {
        emitOp(Op::Code::Null);
    }
    consume(Token::Type::SEMICOLON, "Expect ';' after variable declaration.");

    defineVariable(globalNameIndex);
}

void Compiler::variable() {
    namedVariable(parser.previous);
}

std::size_t Compiler::resolveLocalVariable(Token name) {
    std::cout << "Start...\n";
    if (locals.empty()) return (std::size_t)-1;
    for (std::size_t i = locals.size() - 1; i >= 0; --i) {
        std::cout << "i = " << i << "\n";
        if (locals[i].name == name) {
            if (locals[i].scopeDepth == (std::size_t)-1) {
                error(parser.current, "Variable cannot be initialised with itself!");
            }
            return i;
        }
    }
    return (std::size_t)-1;
}

void Compiler::namedVariable(const Token &token) {
    Op::Code getOp, setOp, getOp32, setOp32;
    std::size_t arg = resolveLocalVariable(token);
    std::cout << "ARG: " << arg << "\n";
    std::cout << "";
    if (arg == (std::size_t)-1) {
        getOp   = Op::Code::GetLocal;
        getOp32 = Op::Code::GetLocal32;
        setOp   = Op::Code::SetLocal;
        setOp32 = Op::Code::SetLocal32;
    }
    else {
        arg = identifierConstant(token);
        getOp   = Op::Code::GetGlobal;
        getOp32 = Op::Code::GetGlobal32;
        setOp   = Op::Code::SetGlobal;
        setOp32 = Op::Code::SetGlobal32;
    }

    if (canAssign and matchType(Token::Type::EQUAL)) {
        expression();
        //Set Global
        if (arg == (std::size_t)-1) {
            error(parser.previous, "Error setting global variable: Out of address space.");
        }
        else if (arg >= 1 << (sizeof(Op::Code) * 8)) {
            emitOp(setOp32);
            auto indexAsOpCodes = Memory::toOpCodes(arg);
            emitOps(indexAsOpCodes[0], indexAsOpCodes[1]);
        }
        else {
            emitOp(setOp);
            emitOp((Op::Code)arg);
        }
    }
    else {
        //Get Global
        if (arg == (std::size_t)-1) {
            error(parser.previous, "Error defining global variable: Out of address space.");
        }
        else if (arg >= 1 << (sizeof(Op::Code) * 8)) {
            emitOp(getOp32);
            auto indexAsOpCodes = Memory::toOpCodes(arg);
            emitOps(indexAsOpCodes[0], indexAsOpCodes[1]);
        }
        else {
            emitOp(getOp);
            emitOp((Op::Code)arg);
        }
    }
}

void Compiler::synchronise() {
    parser.panicMode = false;

    while (parser.current.type != Token::Type::END_OF_FILE) {
        if (parser.previous.type == Token::Type::SEMICOLON) return;
        switch (parser.current.type) {
            case Token::Type::CLASS:
            case Token::Type::FUN:
            case Token::Type::VAR:
            case Token::Type::FOR:
            case Token::Type::IF:
            case Token::Type::WHILE:
            case Token::Type::PRINT:
            case Token::Type::RETURN:
                return;

            default: ; // Do nothing.
        }

        advance();
    }
}

void Compiler::parsePrecedence(Compiler::Precedence precedence) {
    advance();
    ParseFunction prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == nullptr) {
        error(parser.previous, "Expect expression.");
        return;
    }

    canAssign = precedence <= Precedence::ASSIGNMENT;
    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFunction infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }

    if (canAssign && matchType(Token::Type::EQUAL)) {
        error(parser.previous, "Invalid assignment target.");
    }
}

size_t Compiler::parseVariable(const std::string &errorMessage) {
    consume(Token::Type::IDENTIFIER, errorMessage);

    declareVariable();
    if (scopeDepth > 0) return 0;

    return identifierConstant(parser.previous);
}

size_t Compiler::identifierConstant(const Token &token) {
    return emitConstant(Value::fromString(titanSourceCode.substr(token.start, token.length)));
}

void Compiler::addLocalVariable(Token name) {
    //Check if we overflow the local variable array.
    if (locals.size() == (std::size_t)-1) {
        error(parser.previous, "Too many local variables in function.");
    }
    //Check if an identically-named variable exists in the scame scope level.
    for (std::size_t i = locals.size() - 1; i >= 0; --i) {
        if (locals[i].scopeDepth != -1 and locals[i].scopeDepth < scopeDepth) {
            break;
        }
        if (locals[i].name.start == name.start and locals[i].name.length == name.length) {
            error(parser.previous, "A variable with name '" + titanSourceCode.substr(name.start, name.length) + "' already exists in the current scope.");
        }
    }

    Local local;
    local.name = name;
    local.scopeDepth = scopeDepth;
    locals.push_back(local);
}

void Compiler::declareVariable() {
    if (scopeDepth == 0) return; //Scope level 0 is global scope, so return.

    Token* name = &parser.previous;
    addLocalVariable(*name);
}

void Compiler::defineVariable(size_t global) {
    if (scopeDepth > 0) {
        locals.back().scopeDepth = scopeDepth;
        return;
    }
    if (global == (size_t)-1) {
        error(parser.previous, "Error defining global variable: Out of 32-bit address space.");
    }
    else if (global >= 1 << (sizeof(Op::Code) * 8)) {
        emitOp(Op::Code::DefineGlobal32);
        auto indexAsOpCodes = Memory::toOpCodes(global);
        emitOps(indexAsOpCodes[0], indexAsOpCodes[1]);
    }
    else {
        emitOp(Op::Code::DefineGlobal);
        emitOp((Op::Code)global);
    }
}

size_t Compiler::emitConstant(const Value& value) {
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
    return constantIndex;
}

bool Compiler::matchType(Token::Type type) {
    if (parser.current.type != type) return false;
    advance();
    return true;
}

void Compiler::beginScope() {
    scopeDepth++;
}

void Compiler::endScope() {
    scopeDepth--;

    //Pop local variables when they're no longer in scope.
    std::size_t popCount = 0;
    for (; !locals.empty() and locals.back().scopeDepth > scopeDepth; ++popCount) {
        locals.pop_back();
    }
    emitOps(Op::Code::PopN, (Op::Code)popCount);
}




