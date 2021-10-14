#ifndef TITANPLUSPLUS_COMPILER_H
#define TITANPLUSPLUS_COMPILER_H

/**
 * @file Compile.h
 * @author Bryn McKerracher
 * @date 7/10/2021
 * @brief Contains the Compiler class header.
 */

#include <string>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <vector>
#include "Batch.h"
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Memory.h"
#include "Debug.h"

//Enable logging of batch contents.
#define DEBUG_PRINT_CODE

/**
 * @class Compiler
 * @brief
 */
class Compiler {
public:
    ///Ordering of precedence values for parsing.
    enum Precedence {
        NONE,
        ASSIGNMENT,  // =
        OR,          // or
        AND,         // and
        EQUALITY,    // == !=
        COMPARISON,  // < > <= >=
        TERM,        // + -
        FACTOR,      // * /
        UNARY,       // ! -
        CALL,        // . ()
        PRIMARY
    };

    /**
     * Initialises the parse rules.
     */
    Compiler();

    /**
     *  Compiles a string of Titan source code into bytecode
     *  to be interpreted by the VM.
     *
     * @brief Compiles a string of Titan source code.
     * @param titanCode A string containing Titan source code.
     * @param batch The batch to compile the bytecode into.
     * @return True if no compilation errors were found, otherwise false.
     */
    bool compile(const std::string& titanCode, Batch& batch);
protected:
    typedef std::function<void()> ParseFunction; ///< Alias for use in Pratt parsing.

    struct ParseRule {
        ParseFunction prefix;                ///< Prefix behaviour.
        ParseFunction infix;                 ///< Infix behaviour.
        Compiler::Precedence precedence;     ///< Precedence for parsing behaviour.
    };

    std::string titanSourceCode;             ///< A copy of the source code we're compiling.
    Scanner scanner;                         ///< Scans tokens from source code during compilation.
    Parser parser;                           ///< Parses tokens produced by the scanner.
    Batch* currentBatch = nullptr;           ///< Current batch being compiled.
    ParseRule parseRules[Token::Type::SIZE]; ///< List of parsing rules indexed by token type.

    /**
     * @brief Returns a pointer to the parsing rule associated with the given token type.
     * @param type The token type to get the associated rule for.
     * @return A pointer to the parsing rule associated with the given token type.
     */
    ParseRule* getRule(Token::Type type);

    /**
    * @brief Asks the given scanner to generate the next token in the stream.
    */
    void advance();

    /**
     * @brief Displays line and token information about a compilation error.
     * @param token The token that caused the compilation error.
     * @param message The error message to be disaplayed.
     */
    void error(Token& token, const std::string& message);

    /**
     * @brief Tries to consume a token of the expected type from the source, reports an error if it can't.
     * @param tokenType The expected type of token to consume.
     * @param message An error message to display is the next token isn't of the expected type.
     */
    void consume(Token::Type tokenType, const std::string& message);

    /**
     * @brief Writes an Op to the current batch.
     * @param op The op to be written to the current batch.
     */
    void emitOp(Op::Code op);

    /**
     * @brief Writes a pair of Ops to the current batch.
     * @param a The first Op to be written.
     * @param b The second Op to be written.
     */
    void emitOps(Op::Code a, Op::Code b);

    /**
     * @brief Parses the next expression in the source code.
     */
    void expression();

    /**
     * @brief Parses a double from the current position in the source code.
     */
    void number();

    /**
     * @brief Parses a bracket grouping.
     */
    void grouping();

    /**
     * @brief Parses a unary expression.
     */
    void unary();

    /**
     * @brief Parses an expression with two operands.
     */
    void binary();

    /**
     * @brief Parses a literal value.
     */
    void literal();

    /**
     *
     * @param precedence
     */
    void parsePrecedence(Precedence precedence);

    /**
     * @brief Adds an Op sequence to the current batch to store a constant.
     * @param value The value to be added.
     */
    void emitConstant(Value value);
};

#endif //TITANPLUSPLUS_COMPILER_H
