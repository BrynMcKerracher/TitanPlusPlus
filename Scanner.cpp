//
// Created by Bryn McKerracher on 8/10/2021.
//

#include <iostream>
#include "Scanner.h"

void Scanner::parse(const std::string &titanSource) {
    titanSourceCode = titanSource;
    start = 0;
    current = 0;
    line = 0;

    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        std::cout << "Size: " << current - start << "\n";
        std::cout << titanSourceCode.substr(token.start, token.length) << "\n";

        if (token.type == Token::Type::END_OF_FILE) break;
    }
}

void Scanner::init(const std::string &titanSource) {
    titanSourceCode = titanSource;
    start = 0;
    current = 0;
    line = 0;
}

Token Scanner::scanToken() {
    skipWhitespace();
    start = current;

    if (current >= titanSourceCode.size()) return {Token::Type::END_OF_FILE, start, current - start, line};

    const char nextChar = advanceIndex();

    if (isAlphaCharacter(nextChar)) {
        return parseIdentifer();
    }
    if (isDigit(nextChar)) {
        return parseNumber();
    }

    switch (nextChar) {
        //1-character tokens
        case '(': return Token(Token::Type::LEFT_PAREN, start, current - start, line);
        case ')': return Token(Token::Type::RIGHT_PAREN, start, current - start, line);
        case '{': return Token(Token::Type::LEFT_BRACE, start, current - start, line);
        case '}': return Token(Token::Type::RIGHT_BRACE, start, current - start, line);
        case ';': return Token(Token::Type::SEMICOLON, start, current - start, line);
        case ',': return Token(Token::Type::COMMA, start, current - start, line);
        case '.': return Token(Token::Type::DOT, start, current - start, line);
        case '-': return Token(Token::Type::MINUS, start, current - start, line);
        case '+': return Token(Token::Type::PLUS, start, current - start, line);
        case '/': return Token(Token::Type::SLASH, start, current - start, line);
        case '*': return Token(Token::Type::STAR, start, current - start, line);
        case '"': return parseString();

        //2-character tokens
        case '!': return Token(match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG, start, current - start, line);
        case '=': return Token(match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL, start, current - start, line);
        case '>': return Token(match('=') ? Token::Type::GREATER_EQUAL : Token::Type::GREATER, start, current - start, line);
        case '<': return Token(match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS, start, current - start, line);

        default: return Token(Token::Type::ERROR, start, current - start, line);
    }
}

bool Scanner::match(char expected) {
    if (current >= titanSourceCode.size() || titanSourceCode[current] != expected) {
        return false;
    }
    current++;
    return true;
}

bool Scanner::isAlphaCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::advanceIndex() {
    return titanSourceCode[current++];
}

char Scanner::peek(int offset) {
    return titanSourceCode[current + offset];
}

void Scanner::skipWhitespace() {
    for (;;) {
        switch (peek()) {
            case ' ':
            case '\r':
            case '\t':
                current++;
                break;
            case '\n':
                line++;
                current++;
                break;
            case '/': {
                if (current + 1 < titanSourceCode.size() && titanSourceCode[current + 1] == '/') {
                    while (current < titanSourceCode.size() && titanSourceCode[current] != '\n') current++;
                }
                else {
                    return;
                }
                break;
            }
            default: return;
        }
    }
}

Token::Type Scanner::getIdentifierType() {
    //DFA using switches
    switch (titanSourceCode[start]) {
        case 'a': return checkKeyword(1, "nd", Token::Type::AND);
        case 'c': return checkKeyword(1, "lass", Token::Type::CLASS);
        case 'e': return checkKeyword(1, "lse", Token::Type::ELSE);
        case 'f': {
            if (current - start > 1) {
                switch (titanSourceCode[start + 1]) {
                    case 'a': return checkKeyword(2,"lse", Token::Type::FALSE);
                    case 'o': return checkKeyword(2, "r", Token::Type::FOR);
                    case 'u': return checkKeyword(2, "n", Token::Type::FUN);
                }
            }
            break;
        }
        case 'i': return checkKeyword(1, "f", Token::Type::IF);
        case 'n': return checkKeyword(1, "il", Token::Type::NIL);
        case 'o': return checkKeyword(1, "r", Token::Type::OR);
        case 'p': return checkKeyword(1, "rint", Token::Type::PRINT);
        case 'r': return checkKeyword(1, "eturn", Token::Type::RETURN);
        case 's': return checkKeyword(1, "uper", Token::Type::SUPER);
        case 't': {
            if (current - start > 1) {
                switch (titanSourceCode[start + 1]) {
                    case 'h': return checkKeyword(2, "is", Token::Type::THIS);
                    case 'r': return checkKeyword(2, "ue", Token::Type::TRUE);
                }
            }
            break;
        }
        case 'v': return checkKeyword(1, "ar", Token::Type::VAR);
        case 'w': return checkKeyword(1, "hile", Token::Type::WHILE);
    }
    return Token::IDENTIFIER;
}

Token::Type Scanner::checkKeyword(int startIndex, const std::string &name, Token::Type type) {
    if (titanSourceCode.substr(start + startIndex, name.size()) == name) {
        return type;
    }
    return Token::Type::IDENTIFIER;
}

Token Scanner::parseString() {
    while (current < titanSourceCode.size() && titanSourceCode[current] != '"') {
        if (titanSourceCode[current] == '\n') {
            line++;
        }
        current++;
    }

    if (current >= titanSourceCode.size()) {
        return Token(Token::Type::END_OF_FILE, start, current - start, line);
    }

    current++;
    return Token(Token::Type::STRING, start, current - start, line);
}

Token Scanner::parseNumber() {
    while (isDigit(peek())) advanceIndex();

    if (peek() == '.' && isDigit(peek(1))) {
        advanceIndex();

        while (isDigit(peek())) advanceIndex();
    }
    return Token(Token::Type::NUMBER, start, current - start, line);
}

Token Scanner::parseIdentifer() {
    while (isAlphaCharacter(peek()) || isDigit(peek())) advanceIndex();
    return Token(getIdentifierType(), start, current - start, line);
}


