#ifndef TITANPLUSPLUS_VALUE_TPP
#define TITANPLUSPLUS_VALUE_TPP

#include <variant>
#include "Value.h"

template <typename T>
inline T Value::toType() const {
    return std::get<T>(this->data);
}

#endif //TITANPLUSPLUS_VALUE_TPP
