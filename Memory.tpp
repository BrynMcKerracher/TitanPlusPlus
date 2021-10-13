#ifndef TITANPLUSPLUS_MEMORY_TPP
#define TITANPLUSPLUS_MEMORY_TPP

#include "Memory.h"

template <typename T>
std::vector<Op::Code> Memory::toOpCodes(const T& value) {
    std::vector<Op::Code> codes;
    codes.resize(sizeof (value) / sizeof (Op::Code), (Op::Code)0); //Number of codes is ratio of value size to Op::Code size.

    for (int i = 0; i < codes.size(); ++i) {
        codes[i] = (Op::Code)(value >> 8 * sizeof (Op::Code) * i);
    }

    /* Memory Debugging
    for (auto& code : codes) {
        for (int i = 0; i < sizeof (Op::Code) * 8; ++i) {
            std::cout << ((code & (1 << i)) ? "1" : "0");
        }
        //std::cout << ' ';
    }
    std::cout << "\n";
     */

    return codes;
}

template <typename T>
T Memory::toValue(const std::vector<Op::Code> &codes) {
    T value = 0;

    for (int i = 0; i < codes.size(); ++i) {
        value |= (T)(codes[i] << (8 * sizeof (Op::Code) * i));
    }

    /* Memory Debugging
    for (int i = 0; i < sizeof (value) * 8; ++i) {
        std::cout << ((value & (1 << i)) ? "1" : "0");
    }
    std::cout << "\n";
     */

    return value;
}

template<typename T>
T Memory::toValue(Op::Code a, Op::Code b) {
    return toValue<T>(std::vector<Op::Code>{a, b});
}

#endif //TITANPLUSPLUS_MEMORY_TPP
