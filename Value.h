#ifndef TITANPLUSPLUS_VALUE_H
#define TITANPLUSPLUS_VALUE_H

/**
 * @file Value.h
 * @author Bryn McKerracher
 * @date 5/10/2021
 * @brief The Value class, represents a Titan variable.
 */

#include <string>
#include <variant>
#include "Object.h"

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
        STRING, ///< A sequence of characters.
        OBJECT, ///< Heap-allocated object.
    };

    Value::Type type;                                     ///< This Object's value type.
    std::variant<bool, double, std::string, Object> data; ///< Each value instance can only represent one type of value at once.

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
     * @brief Converts a C++ string to a Titan Value object.
     * @param value The string that the Value object will represent.
     * @return A Value object representing the given C++ string.
     */
    static Value fromString(const std::string& value);

    /**
     * @brief Returns the data the provided object represents as the specified type.
     * @tparam T The C++ type to convert the value into.
     * @param value The value to be converted into a C++ type.
     * @return A C++ type representing the object.
     */
    template<typename T>
    static inline T toType(const Value& value);

    /**
     * @brief Returns the data this object represents as the specified type.
     * @tparam T The C++ type to convert the value into.
     * @return A C++ type representing the object.
     */
    template<typename T>
    inline T toType() const;

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

#include "Value.tpp"

#endif //TITANPLUSPLUS_VALUE_H
