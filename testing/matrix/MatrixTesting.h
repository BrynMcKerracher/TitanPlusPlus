//
// Created by Bryn McKerracher on 26/10/2021.
//

#ifndef TITANPLUSPLUS_MATRIXTESTING_H
#define TITANPLUSPLUS_MATRIXTESTING_H

#include <gtest/gtest.h>
#include "../../Matrix.h"

TEST(Matrix, Equal) {
    MatrixF m1("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    MatrixF m2("[8, 108, 6.8, 12.8, 244.669] 8, 108, 6.8, 12.8, 244.669, 8, 108, 6.8, 12.8, 244.669");
    EXPECT_NE(m1, m2);

    m1 = MatrixF("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    m2 = MatrixF("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    EXPECT_EQ(m1, m2);
}

// Test addition
TEST(Matrix, Add) {
    MatrixF m1("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    MatrixF m2("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    MatrixF sum("[8, 108, 6.8, 12.8, 244.669] 8, 108, 6.8, 12.8, 244.669, 8, 108, 6.8, 12.8, 244.669");
    EXPECT_EQ(m1 + m2, sum);

    m1 = MatrixF("[1, 2, 3] 1, 2, 3");
    m2 = MatrixF("[0, 0, 1] 1, 4, 5");
    sum = MatrixF("[1, 2, 4] 2, 6, 8");
    EXPECT_EQ(m1 + m2, sum);

    m1 = MatrixF("[1, 2, 3] 2, 2, 3");
    m2 = MatrixF("[0, 0, 1] 1, 4, 5");
    sum = MatrixF("[1, 2, 4] 2, 6, 8");
    EXPECT_NE(m1 + m2, sum);
}

TEST(Matrix, Subtract) {
    MatrixF m1("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    MatrixF m2("[4, 54, 3.4, 6.4, 122.3345] 4, 54, 3.4, 6.4, 122.3345 4, 54, 3.4, 6.4, 122.3345");
    EXPECT_EQ(m1 - m2, MatrixF::zero(5, 3));
}

TEST(Matrix, Transpose) {
    MatrixF m1("[6, 4.86, 4.52, 8.677] 3, 1000, 1010, 20000");
    MatrixF transpose("[6, 3] 4.86, 1000, 4.52, 1010, 8.677, 20000");
    EXPECT_EQ(m1.transpose(), transpose);

    m1 = MatrixF("[6, 3] 4.86, 1000, 4.52, 1010, 8.677, 20000");
    transpose = MatrixF("[6, 4.86, 4.52, 8.677] 3, 1000, 1010, 20000");
    EXPECT_EQ(m1.transpose(), transpose);

    m1 = MatrixF("[3, 4] 2, 9");
    transpose = MatrixF("[3, 4] 2, 9");
    EXPECT_NE(m1.transpose(), transpose);

    m1 = MatrixF::identity(10000);
    transpose = m1.transpose();
    EXPECT_EQ(m1, transpose);
}

#endif //TITANPLUSPLUS_MATRIXTESTING_H
