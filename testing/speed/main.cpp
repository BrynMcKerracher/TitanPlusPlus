#include <iostream>
#include "../../Matrix.h"

int main() {
    MatrixD matrix = MatrixD::identity(20000);
    auto m2 = matrix.transpose();
    return 0;
}
