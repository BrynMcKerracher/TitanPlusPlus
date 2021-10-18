#ifndef TITANPLUSPLUS_SCANNER_H
#define TITANPLUSPLUS_SCANNER_H

/**
 * @file Scanner.h
 * @author Bryn McKerracher
 * @date 8/10/2021
 * @brief Contains the Scanner class header.
 */

#include <string>
#include "Token.h"

/**
 * Provides methods for lexically parsing Titan source code into
 * tokens.
 *
 * @class Scanner
 * @brief Provides methods for parsing Titan source code.
 */
class Scanner {
public:
    /**
     * Parses a string of Titan source code using lexical parsing.
     *
     * @brief Parses a string of Titan source code.
     * @param titanSource A string of Titan source code to be parsed.
     */
    void parse(const std::string& titanSource);

    /**
     * @brief Sets the parsing string to a copy of the provided source code.
     * @param titanSource A source of Titan source code to be parsed.
     */
    void init(const std::string& titanSource);

    /**
      *  Returns the next Token created through the process of lexical parsing
      *  from the provided Titan source code.
      *
      * @brief Creates and returns the next token generated from the source code.
      * @return The next token generated by lexical parsing.
      */
    Token scanToken();
protected:
    std::string titanSourceCode;     ///< The current source code being analysed.
    size_t start = 0;                ///< Index of starting character of the current lexeme.
    size_t current = 0;              ///< Index of the current character being analysed in the current lexeme.
    int line = 0;                    ///< Line number of the current lexeme.

    /**
     * The function consumes the matched character if a match is detected,
     * otherwise the next character in sequences remains unconsumed.
     *
     * @brief Tests whether the next character in the source code sequences matches the argument.
     * @param expected The character we're matching against the next in the sequence.
     * @return True if the next character in sequence matches our argument.
     */
    inline bool match(char expected);

    /**
     * @brief Tests whether a given character is a letter or underscore.
     * @param c The character to be examined.
     * @return True if the argument is a letter or underscore, false otherwise.
     */
    static inline bool isAlphaCharacter(char c);

    /**
     * Tests whether a given ASCII character is in the range ['0', '9']
     *
     * @brief Test whether a character is a digit.
     * @param c The character to be tested.
     * @return True if the character is a numeric digit, otherwise false.
     */
    static inline bool isDigit(char c);

    /**
     * @brief Increments 'current' and returns the character before it in the code.
     * @return The character at the current index.
     */
    char advanceIndex();

    /**
     * @brief Returns the character at the current position plus an offset.
     * @param offset The number of characters to look forward or backwards.
     * @return The character in the source at position 'current + offset'.
     */
    char peek(int offset = 0);

    /**
     * Skips all contiguous whitespace immediately following the current character in the
     * source sequence. Stops when it reaches the next non-whitespace character.
     *
     * @brief Skips the next whitespace section in the Titan source code sequence.
     */
    void skipWhitespace();

    /**
     * @brief Returns the token type matching the reserve keyword.
     * @return The token type matched the reserved keyword.
     */
    Token::Type getIdentifierType();

    /**
     * Checks if the given keyword is present at the provided location in the source code,
     *
     * @brief Checks if the given keyword is present at the provided location in the source code.
     * @param startIndex The number of characters after 'start' to begin checking for a keyword.
     * @param name The keyword to check against.
     * @param type The token type to return if the provided keyword is found.
     * @return The token type for the given keyword if found, otherwise returns Token::Type::Identifier.
     */
    Token::Type checkKeyword(int startIndex, const std::string &name, Token::Type type);

    /**
     * Parses a string from the current position in the Titan source
     * sequence.
     *
     * @brief Scans the next token as a string.
     * @return A token identifying the string.
     */
    Token parseString();

    /**
     * Parses a numeric literal from the current position in the Titan source
     * sequence.
     *
     * @brief Scans the next token as a numeric literal.
     * @return A token identifying the literal.
     */
    Token parseNumber();

    /**
     * Parses an identifier from the current position in the Titan source
     * sequence.
     *
     * @brief Scans the next token as an identifier.
     * @return A token representing an identifier.
     */
    Token parseIdentifer();
};

#endif //TITANPLUSPLUS_SCANNER_H