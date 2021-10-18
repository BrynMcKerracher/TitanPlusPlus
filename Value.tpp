#ifndef TITANPLUSPLUS_VALUE_TPP
#define TITANPLUSPLUS_VALUE_TPP

#include <variant>

template <typename T>
inline T Value::toType(const Value &value) {
    return std::get<T>(value.data);
}

template <typename T>
inline T Value::toType() const {
    return std::get<T>(this->data);
}

#endif //TITANPLUSPLUS_VALUE_TPP
