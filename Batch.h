#ifndef TITANPLUSPLUS_BATCH_H
#define TITANPLUSPLUS_BATCH_H

/**
 * @file Batch.h
 * @author Bryn McKerracher
 * @date 4/10/2021
 * @brief The Batch class, contains information about a code batch.
 */

#include <vector>
#include "common.h"
#include "Ops.h"
#include "Value.h"

/**
 * Contains the bytecode stream and constant pool for a given set of
 * Titan statements.
 *
 * The compiler creates these from parsing information for the VM to interpret.
 *
 * @class Batch Contains information about a batch of code.
 * @brief Holds the constant pool and bytecode stream for a code batch.
 */
struct Batch {
public:
    std::vector<Op::Code> opcodes;   ///< The bytestream of Op::Codes for this batch.
    std::vector<Value> constantPool; ///< The list of all constants defined in this batch.
    std::vector<int> lines;          ///< Line numbers for all instructions (One line number per instruction).

    /**
     * Pushes an Op::Code to the back of the bytestream.
     *
     * @brief Adds an Op::Code to the back of the bytestream.
     * @param op The Op::Code to be pushed onto the back of the bytestream.
     * @param lineNum The line number of the Op::Code sequence.
     */
    void addOp(Op::Code op, int lineNum);

    /**
     * Adds a sequence of codes (in order of first to last) to the back
     * of the bytestream.
     *
     * This is mostly useful for adding values that span a sequence of
     * Op::Codes, such as those created by Memory::toOpCodes().
     *
     * @brief Adds a sequence of codes to the bytestream.
     * @param codes The codes to be added to the bytestream.
     * @param lineNum The line number of the Op::Code sequence.
     */
    void addOps(const std::vector<Op::Code>& codes, int lineNum);

    /**
     * Pushes a constant onto the back of the constant pool and returns its index.
     *
     * @brief Adds a constant to the constant pool.
     * @param value The constant to be added to the pool.
     * @return The index of the constant in the pool.
     */
    size_t addConstant(Value value);
};

#endif //TITANPLUSPLUS_BATCH_H
