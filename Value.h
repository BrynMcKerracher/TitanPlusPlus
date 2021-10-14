#ifndef TITANPLUSPLUS_VALUE_H
#define TITANPLUSPLUS_VALUE_H

/**
 * @file Value.h
 * @author Bryn McKerracher
 * @date 5/10/2021
 * @brief The Value class, represents a Titan variable.
 */

#include <string>

/**
 * Value is essentially a tagged-union structure with utility functions.
 *
 * @note A Titan Value instance can only ever be one value type at any point in time,
 * so a union is appropriate, however this means developers must check that the types
 * of Values match if performing operations with multiple values.
 *
 * @brief Represents a variable or value in Titan.
 * @class Value
 */
struct Value {
    ///< Exhaustive enumeration of supported value types.
    enum Type {
        BOOL,   ///< Boolean value.
        NIL,    ///< Null value.
        NUMBER, ///< Numeric value (double precision).
        OBJECT, ///< Heap-allocated object.
    };

    Value::Type type; ///< This Object's value type.

    ///< Each value instance can only represent one type of value at once.
    union {
        bool boolean;
        double number;
    } as;

    /**
     * @brief Converts a C++ boolean value to a Titan Value object.
     * @param value The boolean value the Value object will represent.
     * @return A Value object representing the given boolean.
     */
    static Value fromBool(bool value);

    /**
     * @brief Converts a null value to a Titan Value object.
     * @return A Value object representing null.
     */
    static Value fromNull();

    /**
     * @brief Converts a C++ double value to a Titan Value object.
     * @param value The double value the Value object will represent.
     * @return A Value object representing the given C++ double.
     */
    static Value fromNumber(double value);

    /**
     * @brief Converts a Titan value to a C++ boolean.
     * @param value The Titan Value object to be converted to a C++ boolean.
     * @return The C++ boolean representation of the provided Titan Value object;
     */
    static inline bool toBool(const Value& value);

    /**
     * @brief Converts a Titan value to a C++ double.
     * @param value The Titan Value object to be converted to a C++ double.
     * @return The C++ double representation of the provided Titan Value object;
     */
    static inline double toNumber(const Value& value);

    /**
     * @brief Creates a string representation of this object.
     * @return A string representation of the Titan Value.
     */
    std::string toString() const;

    /**
     * @brief Tests the equality of this object with another value.
     * @param rhs The value to compare against.
     * @return True if the values are equal, otherwise false
     */
    bool operator==(const Value& rhs) const;
};

#endif //TITANPLUSPLUS_VALUE_H
