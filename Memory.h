#ifndef TITANPLUSPLUS_MEMORY_H
#define TITANPLUSPLUS_MEMORY_H

/**
 * @file Memory.h
 * @author Bryn McKerracher
 * @date 5/10/2021
 * @brief Contains the definition for the memory class.
 */

#include <vector>
#include <iostream>
#include "Ops.h"

/**
 * Memory does low-level bit manipulation for converting Op::Codes operands to
 * and from various types.
 *
 * It's often easier to work with size_t and other standard C++ types when developing
 * the backend, so consider Memory a utility class that allows us to convert between these
 * types and the bread and butter of Titan's world, Op::Code.
 *
 * @class Memory
 * @brief Contains methods for converting bytecode to/from various types.
 */
struct Memory {
    /**
     *  Takes a given value and splits its bits across a number of Op::Codes,
     *  then returns these codes as a vector.
     *
     *  The sequence of Op::Codes generated will follow the exact memory pattern
     *  of value. This allows us to easily convert back to a T-type value using
     *  toValue(). It may be helpful to think of this function as
     *  "slicing" the value into Op::Code-sized pieces.
     *
     * @brief Splits the bits of a given value into a sequence of Op::Codes.
     * @tparam T The type of value to be split into Op::Codes.
     * @param value The value to be split into Op::Codes.
     * @return A vector of Op::Codes that match the bit sequence in value.
     */
    template<typename T>
    static std::vector<Op::Code> toOpCodes(const T& value);

    /**
     * Converts a series of Op::Codes into a T-type value.
     *
     * The memory of value will be an exact in-order match to the memory of the
     * sequence of Op::Codes. It may be helpful to think of this function as
     * 'stitching together' the Op::Codes into a single value.
     *
     * @brief Converts an Op::Code sequence into a value.
     * @tparam T The type to convert the Op::Code sequence into.
     * @param codes The vector of Op::Codes to be converted into a value.
     * @return A T-type value whose memory is an exact match of the Op::Code sequence.
     */
    template <typename T>
    static T toValue(const std::vector<Op::Code>& codes);

    /**
     * Calls Memory::toValue(const std::string<Op::Code>&) after
     * first converting the two values (a and b) into a vector of
     * Op::Codes in the order a, b.
     *
     * This is a convenience function as quite often we want to combine
     * two Op::Codes into a value and converting these values to a
     * std::vector first can get quite verbose.
     *
     * @brief Converts two Op::Codes into a value.
     * @tparam T The type to convert the Op::Code sequence into.
     * @param a The first Op::Code to be converted into a value.
     * @param b The second Op::Code to be converted into a value.
     * @return A T-type value whose memory is an exact match of the Op::Code sequence a, b.
     */
    template<typename T>
    static T toValue(Op::Code a, Op::Code b);
};

#include "Memory.tpp"

#endif //TITANPLUSPLUS_MEMORY_H
