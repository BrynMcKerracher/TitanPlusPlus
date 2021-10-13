#ifndef TITANPLUSPLUS_PARSER_H
#define TITANPLUSPLUS_PARSER_H

/**
 * @file Parser.h
 * @author Bryn McKerracher
 * @date 12/10/2021
 * @brief Contains the Parser class header.
 */

#include "Token.h"

/**
 * @class Parser
 * @brief Utility class for the Compiler that contains info about the current state of parsing.
 */
class Parser {
public:
    Token current;            ///< The current token being parsed.
    Token previous;           ///< The previous token being parsed.
    bool hadError = false;    ///< Whether or not the parser has encountered a parsing error.
    bool panicMode = false;   ///< If the parser should ignore future errors.
};

#endif //TITANPLUSPLUS_PARSER_H
