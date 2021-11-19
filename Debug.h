#ifndef TITANPLUSPLUS_DEBUG_H
#define TITANPLUSPLUS_DEBUG_H

/**
 * @file Debug.h
 * @author Bryn McKerracher
 * @date 4/10/2021
 * @brief The Debug class, contains methods for deconstructing bytecode.
 */

#include <iostream> //For std::cout
#include "Ops.h"
#include "Batch.h"

/**
 * @class Debug
 * @brief Contains methods for deconstructing bytecode into human-readable strings.
 *
 * Contains methods for debugging the compiler, scanner, and VM.
 */
struct Debug {
    /**
     * Goes through the instructions contained in the provided batch and
     * calls disassembleInstruction() on each of those instructions.
     *
     * @brief Disassembles the instructions of the provided batch.
     * @param batch The batch to have its instructions disassembled.
     */
    static void disassembleBatch(const Batch& batch);

    /**
     * Prints the index of the instruction in the bytecode stream, then
     * returns the index of the next instruction in the stream.
     *
     * Some bytecode instructions have an operand (or multiple), so it's
     * not guaranteed that every byte in the instruction stream is a instruction.
     *
     * @brief Prints information about the specified instruction and returns the index of the next instruction.
     * @param batch The batch containing the instruction to be disassembled.
     * @param instructionIndex The index of the instruction to be disassembled.
     * @return The index of the next instruction in the bytecode stream.
     */
    static int disassembleInstruction(const Batch& batch, size_t instructionIndex);
};


#endif //TITANPLUSPLUS_DEBUG_H
