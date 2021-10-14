#ifndef TITANPLUSPLUS_VM_H
#define TITANPLUSPLUS_VM_H

/**
 * @file VM.h
 * @author Bryn McKerracher
 * @date 5/10/2021
 * @brief Contains the definition for Virtual Machine.
 */

#include <iostream>
#include "Batch.h"
#include "Ops.h"
#include "Memory.h"
#include "Debug.h"
#include "Compiler.h"
#include "Value.h"

//Enable verbose tracing of bytecode execution
#define DEBUG_TRACE_EXECUTION

/**
 * The meat of Titan.
 *
 * Interprets the bytestream and executes instructions.
 *
 * @class VM Provides all functionality for executing bytecode.
 * @brief The Titan Virtual Machine. Interprets Titan bytecode.
 */
class VM {
public:
    ///Possible results of interpreting a batch.
    enum InterpretResult {
        OK,             ///< No error.
        COMPILE_ERROR,  ///< Error found during compilation.
        RUNTIME_ERROR   ///< Error found during execution.
    };

    /**
     * Reserves stack space for the VM using MaxStackSize.
     */
    VM();

    /**
     * Interprets a string of Titan source code.
     *
     * @brief Interprets Titan source code.
     * @param titanCode
     * @return OK if no errors found, otherwise COMPILE_ERROR or RUNTIME_ERROR.
     */
    InterpretResult interpret(const std::string& titanCode);
protected:
    Op::Code* pc = nullptr;             ///< Program counter.
    std::vector<Value> stack;           ///< The VM's value stack.
    const size_t MaxStackSize = 4096;   ///< The maximum size of the value stack before stack overflow.

    /**
     * Executes the bytecode instructions of a batch.
     *
     * @brief Runs the Titan bytecode of a single batch.
     * @param batch The batch to be run.
     * @return OK if no errors found, otherwise COMPILE_ERROR or RUNTIME_ERROR.
     */
    InterpretResult run(Batch& batch);

    /**
     * Pops the two back-most values from the stack
     * and returns them as a vector.
     *
     * This is a helper-function for operations that have two operands
     * intended to reduce verbosity in the dispatch loop (run()).
     *
     * @brief Pops and returns the two back-most stack values.
     * @return The two back-most values on the stack.
     */
    inline std::vector<Value> popBinaryOperands();

    /**
     * Pops and returns a copy of the back-most value on the stack.
     *
     * This is a helper function for reducing the verbosity of
     * popping a value from the stack where the intention is to
     * retain its value.
     *
     * @brief Pops and returns the back-most value on the stack.
     * @return The back-most value on the stack.
     */
    inline Value popValue();

    /**
     * @brief Prints an error, resets the stack and halts runtime execution.
     * @param format The error string to print.
     * @param ...
     */
    void runtimeError(const std::string& format, Batch& batch);

    /**
     * @brief Returns true if the value can be evaluated to false.
     * @param value The value to be tested for falsiness.
     * @return True if the value evaluates to false, otherwise false.
     */
    bool isFalsey(const Value& value);
};


#endif //TITANPLUSPLUS_VM_H
