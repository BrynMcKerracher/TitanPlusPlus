#include "Value.h"

Value Value::fromBool(bool value) {
    return {Value::Type::BOOL, value};
}

Value Value::fromNull() {
    return {Value::Type::NIL, 0.f};
}

Value Value::fromNumber(double value) {
    return {Value::Type::NUMBER, value};
}

Value Value::fromString(const std::string &value) {
    return {Value::Type::STRING, value};
}

Value Value::fromMatrixF(const MatrixF &value) {
    return {Value::Type::MATRIXF, value};
}

Value Value::fromMatrixD(const MatrixD &value) {
    return {Value::Type::MATRIXD, value};
}

std::string Value::toString() const {
    switch (type) {
        case Type::BOOL:    return std::get<bool>(data) ? "true" : "false";
        case Type::NIL:     return "null";
        case Type::NUMBER:  return std::to_string(std::get<double>(data));
        case Type::STRING:  return std::get<std::string>(data);
        case Type::MATRIXF: return std::get<MatrixF>(data).toString();
        case Type::MATRIXD: return std::get<MatrixD>(data).toString();
        default:            return "Unknown type.";
    }
}

bool Value::operator==(const Value &rhs) const {
    if (this->type != rhs.type) return false;
    switch (this->type) {
        case Type::BOOL:    return std::get<bool>(data) == std::get<bool>(rhs.data);
        case Type::NIL:     return true;
        case Type::NUMBER:  return std::get<double>(data) == std::get<double>(rhs.data);
        case Type::STRING:  return std::get<std::string>(data) == std::get<std::string>(rhs.data);
        case Type::MATRIXF: return std::get<MatrixF>(data) == std::get<MatrixF>(rhs.data);
        case Type::MATRIXD: return std::get<MatrixD>(data) == std::get<MatrixD>(rhs.data);
        default:            return false;
    }
}

std::string Value::typeToString(Value::Type type) {
    switch (type) {
        case Type::BOOL:    return "BOOL";
        case Type::NIL:     return "NULL";
        case Type::NUMBER:  return "NUMBER";
        case Type::STRING:  return "STRING";
        case Type::MATRIXF: return "MATRIXF";
        case Type::MATRIXD: return "MATRIXD";
        default:            return "Unknown type.";
    }
}


