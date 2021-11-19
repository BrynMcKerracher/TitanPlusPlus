//
// Created by Bryn McKerracher on 17/11/2021.
//

#ifndef TITANPLUSPLUS_VARIABLETEST_H
#define TITANPLUSPLUS_VARIABLETEST_H

#include <gtest/gtest.h>
#include <sstream>
#include "../../VM.h"

TEST(VM, VariableAddress) {
    VM vm;

    std::stringstream titanSource;
    for (size_t i = 0; i < 70000; ++i) {
        titanSource << "var test" << i << " = 1;";
    }
    vm.interpret(titanSource.str());
}

#endif //TITANPLUSPLUS_VARIABLETEST_H
