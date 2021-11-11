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
#include "Local.h"

//Enable logging of batch contents.
#define DEBUG_PRINT_CODE

/**
 * @class Compiler
 * @brief
 */
class Compiler {
public:
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
    bool canAssign = true;                   ///< Whether or not the expression being currently evaluated can be assigned to.
    std::vector<Local> locals;               ///< Array of all local variables, assigned at compile-time.
    std::size_t scopeDepth = 0;              ///< Depth of the current scope during compilation.

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
     * @brief Emits a jump instruction.
     * @param jumpOp The jump operation to emit.
     * @return The index of the first operand of the jump instruction, for use in back-patching.
     */
    Op::Code emitJump(Op::Code jumpOp);

    /**
     * @brief Back-patches the destination of a jump operation into the bytecode.
     * @param offset The location of the jump instruction's first operand.
     */
    void backPatchJump(Op::Code offset);

    /**
     * @brief Parses the next expression in the source code.
     */
    void expression();

    /**
     * @brief Parses a block.
     */
    void block();

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
     * @brief Parses a string.
     */
     void string();

     /**
      * @brief Parses a matrix.
      */
     void matrix();

     /**
      * @brief Parses a declaration.
      */
     void declaration();

     /**
      * @brief Parses a statement.
      */
     void statement();

     /**
      * @brief Parses an expression and pops the resulting value from the stack.
      */
     void expressionStatement();

     /**
      * @brief Parses an if-statement.
      */
     void ifStatement();

     /**
      * @brief Parses a variable declaration.
      */
     void variableDeclaration();

     /**
      * @brief Parses a variable.
      */
     void variable();

     /**
      * @brief Returns the index of a local variable with the given name, or -1 if not found.
      * @param name The token representing the name of the variable.
      * @return The index of the local variable in the locals array, or -1 if not found.
      */
     std::size_t resolveLocalVariable(Token name);

     /**
      * @brief Creates a sequence of instructions for loading a named variable.
      * @param token The token to parse the variable from.
      */
     void namedVariable(const Token& token);

     /**
      * @brief Recovers the parser from panic mode to prevent cascade errors.
      */
     void synchronise();

    /**
     * @brief Parses a token from source code at the given precedence.
     * @param precedence Parses a token from source with the given precedence.
     */
    void parsePrecedence(Precedence precedence);

    /**
     * @brief Parses a variable name from the source stream.
     * @param errorMessage Message to be displayed if parsing fails.
     * @return The index of the new variable's name in the constants array of the current batch.
     */
    size_t parseVariable(const std::string& errorMessage);

    /**
     * @brief Creates a string constant and returns its index for use in defining variables.
     * @param token Token to lex the constant's name from.
     * @return The index of the string.
     */
    size_t identifierConstant(const Token& token);

    /**
     * @brief Registers a local variable name in the locals pool at the current scope-depth.
     * @param name The token containing the name to be registered.
     */
    void addLocalVariable(Token name);

    /**
     * @brief Declares a local variable, which occurs during compile-time.
     */
    void declareVariable();

    /**
     * @brief Writes Op::Codes to the current batch to define a global variable, given the index to its name.
     * @param global The index of the global variable's name in the batch's constant pool.
     */
    void defineVariable(size_t global);

    /**
     * @brief Adds an Op sequence to the current batch to store a constant.
     * @param value The value to be added.
     * @return The index of the constant in the current batch's constant table.
     */
    size_t emitConstant(const Value& value);

    /**
     * @brief Consumes a token if it has the given type, otherwise returns false.
     * @param type The type to check the next token for.
     * @return True if the next token matches the given type, otherwise false.
     */
    bool matchType(Token::Type type);

    /**
     * @brief Increments the scope depth counter.
     */
    void beginScope();

    /**
     * @brief Decrements the scope depth counter.
     */
    void endScope();
};

#endif //TITANPLUSPLUS_COMPILER_H
