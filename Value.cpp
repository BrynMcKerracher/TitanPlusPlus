#include "Value.h"

Value Value::fromBool(bool value) {
    return (Value){Value::Type::BOOL, {.boolean = value}};
}

Value Value::fromNull() {
    return (Value){Value::Type::NIL, {.number = 0}};
}

Value Value::fromNumber(double value) {
    return (Value){Value::Type::NUMBER, {.number = value}};
}

bool Value::toBool(const Value &value) {
    return value.as.boolean;
}

double Value::toNumber(const Value &value) {
    return value.as.number;
}

std::string Value::toString() const {
    switch (type) {
        case Type::BOOL:   return as.boolean ? "true" : "false";
        case Type::NIL:    return "null";
        case Type::NUMBER: return std::to_string(this->as.number);
        default:           return "Unknown type.";
    }
}

bool Value::operator==(const Value &rhs) const {
    if (this->type != rhs.type) return false;
    switch (this->type) {
        case Value::Type::BOOL:     return this->as.boolean == rhs.as.boolean;
        case Value::Type::NIL:      return true;
        case Value::Type::NUMBER:   return this->as.number == rhs.as.number;
        default: return false;
    }
}


