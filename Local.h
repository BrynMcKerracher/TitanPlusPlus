#ifndef TITANPLUSPLUS_LOCAL_H
#define TITANPLUSPLUS_LOCAL_H

/**
 * @file Local.h
 * @author Bryn McKerracher
 * @date 8/11/2021
 * @brief Definition of a local variable object.
 */

#include "Token.h"

/**
 * @struct Local
 * @brief Represents a local variable instance.
 * @note Local variables are not late-bound, meaning that all locals are declared at compile-time.
 */
struct Local {
    Token name;
    std::size_t scopeDepth = (std::size_t)-1;
};

#endif //TITANPLUSPLUS_LOCAL_H
