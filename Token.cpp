//
// Created by Bryn McKerracher on 8/10/2021.
//

#include "Token.h"

Token::Token(size_t index, const std::string& titanSource) {
    start = index;

    if (index >= titanSource.size()) {
        type = Token::Type::END_OF_FILE;
        return;
    }

    type = Token::Type::ERROR;
}

Token::Token(Token::Type tokenType, size_t index, size_t tokenLength, int tokenLine) {
    type = tokenType;
    start = index;
    length = tokenLength;
    line = tokenLine;
}
