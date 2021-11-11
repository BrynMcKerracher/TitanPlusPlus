#ifndef TITANPLUSPLUS_TOKEN_H
#define TITANPLUSPLUS_TOKEN_H

/**
 * @file Token.h
 * @author Bryn McKerracher
 * @date 8/10/2021
 * @brief Contains the Token class header.
 */

#include <cstdint>
#include <string>

/**
 * An object for holding and accessing information about a single lexeme.
 *
 * This is a utility class for the Scanner class's lexical parsing of
 * Titan source code. Token objects are a midpoint between raw Titan
 * source code and the VM's bytecode.
 *
 * @class Token
 * @brief A utility class for lexical parsing.
 */
class Token {
public:
    /**
     * A comprehensive list of all tokens supported by the parser.
     *
     * @enum Type
     * @brief A enumeration of all supported token types.
     */
    enum Type {
        LEFT_PAREN, RIGHT_PAREN,
        LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS,
        SEMICOLON, SLASH, STAR,

        // One or two character tokens.
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        // Literals.
        IDENTIFIER, STRING, NUMBER, MATRIX,

        // Keywords.
        AND, CLASS, ELSE, FALSE,
        FOR, FUN, IF, NIL, OR,
        PRINT, RETURN, SUPER, THIS,
        TRUE, VAR, WHILE,

        //Errors
        ERROR, END_OF_FILE,

        //Number of types (Must be last)
        SIZE,
    };

    Token::Type type = Token::Type::ERROR;    ///< The type of token this object represents.
    size_t start = 0;    ///< Pointer to the start character of the token in the source code.
    size_t length = 0;   ///< The length of the token in ASCII characters.
    int line = 0;        ///< The line number of the token.

    /**
     * Creates a token object using the given index for the starting
     * character of the token in the source code.
     *
     * @brief Creates a token object from the source code.
     * @param index The index of the Token's starting character in the source code.
     * @param titanSource The string of Titan source code to generate a token from.
     */
    Token(size_t index, const std::string& titanSource);

    /**
     * Creates a token.
     *
     * @brief Constructs a token from the given type.
     * @param type The type of token to be constructed.
     * @param index The index of the Token's starting character in the source code.
     * @param tokenLength The length of the token in ASCII characters.
     * @param line The line number of the token.
     */
    Token(Token::Type tokenType, size_t index, size_t tokenLength, int tokenLine);

    /**
     * @brief Default constructor.
     */
    Token() = default;

    /**
     * @brief Tests equality with another token.
     * @param rhs The token to compare with.
     * @return True if tokens represent the same substring in the source code, otherwise false.
     */
    bool operator==(const Token& rhs) const;
};


#endif //TITANPLUSPLUS_TOKEN_H
