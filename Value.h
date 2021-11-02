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
#include "Matrix.h"

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
        MATRIXF, ///< A numeric matrix of floats.
        MATRIXD, ///< A numeric matrix of doubles.
    };

    Value::Type type = Value::Type::NIL;                            ///< This Object's value type.
    std::variant<bool, double, std::string, MatrixF, MatrixD> data; ///< Each value instance can only represent one type of value at once.

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
     * @brief Converts a MatrixF to a Titan Value object.
     * @param value The matrix to be converted.
     * @return A Value object representing the given MatrixF.
     */
    static Value fromMatrixF(const MatrixF& value);

    /**
     * @brief Converts a MatrixD to a Titan Value object.
     * @param value The matrix to be converted.
     * @return A Value object representing the given MatrixD.
     */
    static Value fromMatrixD(const MatrixD& value);

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

    /**
     * For use printing human readable errors when exceptions are thrown by operands with
     * incompatible types.
     *
     * @brief Creates a human readable std::string representation of the given Value::Type.
     * @param type The value type to convert to std::string.
     * @return A human readable std::string representation of the given Value::Type.
     */
    static std::string typeToString(Value::Type type);
};

#include "Value.tpp"

#endif //TITANPLUSPLUS_VALUE_H
