#ifndef TITANPLUSPLUS_OPS_H
#define TITANPLUSPLUS_OPS_H

/**
 * @file Ops.h
 * @author Bryn McKerracher
 * @date 4/10/2021
 * @brief Definitions of OpCodes
 */

#include <string> //For std::string

/**
 * Utility class that provides information about OpCodes.
 *
 *  @class Op
 *  @brief Contains all methods and data relating to OpCodes.
 */
struct Op {
    ///List of all OpCodes, each is two byte.
    enum Code : uint16_t {
        Add,            ///< Adds and pops the two values at the back of the stack, then pushes the result.
        Constant64,     ///< Load a constant using the next 64 bits of the bytecode stream as the value's index in the constant pool.
        Constant32,     ///< Load a constant using the next 32 bits of the bytecode stream as the value's index in the constant pool.
        Constant,       ///< Load a constant using the next Op::Code in the bytecode stream as the value's index in the constant pool.
        DefineGlobal64, ///< Define a global variable with a 64-bit index in the VM's globals array.
        DefineGlobal32, ///< Define a global variable with a 32-bit index in the VM's globals array.
        DefineGlobal,   ///< Define a global variable in the VM's globals array.
        Divide,         ///< Divides and pops the two values at the back of the stack, then pushes the result.
        Equal,          ///< Tests if the top two values on the stack are equal.
        False,          ///< Represents a boolean 'false' value.
        GetGlobal64,    ///< Loads a 64-bit-addressed global onto the stack.
        GetGlobal32,    ///< Loads a 32-bit-addressed global onto the stack.
        GetGlobal,      ///< Loads a global onto the stack.
        GetLocal64,     ///< Loads a 64-bit addressed local variable onto the stack.
        GetLocal32,     ///< Loads a 32-bit addressed local variable onto the stack.
        GetLocal,       ///< Loads a local variable onto the stack.
        Greater,        ///< Tests the top two values of the stack and returns true if the second-most is greater.
        GreaterEqual,   ///< Tests the top two values of the stack and returns false if the second-most is lesser.
        Jump,           ///< Performs a jump.
        JumpBack,       ///< Performs a jump upwards on the stack.
        JumpIfFalse,    ///< Performs a jump if condition evaluates to false.
        JumpIfFalsePop, ///< Performs a jump if the stack top evaluates to false and then pops the top of the stack.
        Less,           ///< Tests the top two values of the stack and returns true if the second-most is lesser.
        LessEqual,      ///< Tests the top two values of the stack and returns false if the second-most is greater.
        Multiply,       ///< Multiplies and pops the two values at the back of the stack, then pushes the result.
        Negate,         ///< Negate the result from the top of the VM's stack.
        Not,            ///< Logically negate the top of the VM's stack.
        NotEqual,       ///< Tests if the topmost two values on the stack are inequal.
        Null,           ///< Represents a null value.
        Pop,            ///< Pops the top value from the stack.
        PopN,           ///< Pop the top N values from the stack.
        Print,          ///< Prints and pops a value from the stack.
        Return,         ///< Exit from VM processing cycle.
        SetGlobal64,    ///< Set the value of a 64-bit addressed variable.
        SetGlobal32,    ///< Set the value of a 32-bit addressed variable.
        SetGlobal,      ///< Set the value of a global.
        SetLocal64,     ///< Set the value of a 64-bit addressed local variable.
        SetLocal32,     ///< Set the value of a 32-bit addressed local variable.
        SetLocal,       ///< Set the value of a local variable.
        Subtract,       ///< Subtracts and pops the two values at the back of the stack, then pushes the result.
        True,           ///< Represents a boolean 'true' value.
    };

    /**
     *  Returns the number of entries the provided Op::Code and all its operands
     *  will take up in the bytestream.
     *
     *  This is mainly useful for calculating the index of the following instruction
     *  in the bytecode stream.
     *
     * @brief Returns the instruction length of the provided OpCode in instruction indexes.
     * @param op The Op::Code to be analysed.
     * @return The number of instruction indexes the Op::Code and its operands take up in the bytestream.
     */
    static int instructionLength(Code op);

    /**
     *  Provides a human-readable string representation of the provided Op::Code.
     *
     *  This is mostly useful for debugging as a helper function for creating a
     *  human-readable bytecode-stack unwind of the VM.
     *
     * @brief Returns the name of the provided Op::Code.
     * @param op The Op::Code to be analysed.
     * @return The name of the provided Op::Code as a human-readable string.
     */
    static std::string instructionName(Code op);
};

#endif //TITANPLUSPLUS_OPS_H
