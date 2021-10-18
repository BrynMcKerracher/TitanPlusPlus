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

std::string Value::toString() const {
    switch (type) {
        case Type::BOOL:   return std::get<bool>(data) ? "true" : "false";
        case Type::NIL:    return "null";
        case Type::NUMBER: return std::to_string(std::get<double>(data));
        case Type::STRING: return std::get<std::string>(data);
        default:           return "Unknown type.";
    }
}

bool Value::operator==(const Value &rhs) const {
    if (this->type != rhs.type) return false;
    switch (this->type) {
        case Type::BOOL:   return std::get<bool>(this->data) == std::get<bool>(rhs.data);
        case Type::NIL:    return true;
        case Type::NUMBER: return std::get<double>(this->data) == std::get<double>(rhs.data);
        case Type::STRING: return std::get<std::string>(this->data) == std::get<std::string>(rhs.data);
        default:           return false;
    }
}


