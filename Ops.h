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
        Add,        ///< Adds and pops the two values at the back of the stack, then pushes the result.
        Constant32, ///< Load a 32-bit constant from the stream as an index for the constant pool.
        Constant,   ///< Load a constant using the next Op::Code in stream as an index for the constant pool.
        Divide,     ///< Divides and pops the two values at the back of the stack, then pushes the result.
        Multiply,   ///< Multiplies and pops the two values at the back of the stack, then pushes the result.
        Negate,     ///< Negate the result from the top of the VM's stack.
        Return,     ///< Exit from VM processing cycle.
        Subtract,   ///< Subtracts and pops the two values at the back of the stack, then pushes the result.
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
