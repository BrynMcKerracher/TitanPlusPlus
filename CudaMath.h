#ifndef TITANPLUSPLUS_CUDAMATH_H
#define TITANPLUSPLUS_CUDAMATH_H

/**
 * @file CudaMath.cu
 * @author Bryn McKerracher
 * @date 20/10/2021
 * @brief Contains Cuda array math for matrix operations.
 */

#include <utility>
#include <cfloat>

namespace CudaMath {
/**
 * @brief Performs point-wise addition on a and b, and stores the results in sum.
 * @tparam T The type of the elements in the arrays, either float or double.
 * @param n The size of the arrays.
 * @param a Pointer to first summand array.
 * @param b Pointer to second summand array.
 * @param sum Pointer to sum array.
 */
template <typename T>
void cudaAdd(size_t n, T* a, T* b, T* sum);

/**
 * @brief Performs point-wise subtraction on a and b, and stores the results in result.
 * @tparam T The type of the elements in the arrays, either float or double.
 * @param n The size of the arrays.
 * @param a Pointer to minuend array.
 * @param b Pointer to second subtrahend array.
 * @param result Pointer to result array.
 */
template <typename T>
void cudaSubtract(size_t n, T* a, T* b, T* result);

/**
 * @brief Performs point-wise scalar multiplication of a by b, and stores the results in result.
 * @tparam T The type of the elements in a, either float or double.
 * @param n The size of the array in a.
 * @param a Pointer to the array being multiplied.
 * @param b Scalar to multiply by.
 * @param result Pointer to result array.
 */
template <typename T>
void cudaScalarMultiply(size_t n, T* a, T b, T* result);

/**
 * @brief Tests the equality of two matrices.
 * @tparam T The element types of the provided arrays.
 * @param a_n The number of elements in array a.
 * @param b_n The number of elements in array b.
 * @param a Pointer to array a.
 * @param b Pointer to array b.
 * @return True if all elements are equal, otherwise false.
 */
template <typename T>
bool cudaEqual(size_t n, T* a, T* b);

/**
 * @brief Calculates the transpose of a matrix.
 * @tparam T The types of elements in the matrix.
 * @param n The number of elements in the matrix.
 * @param oldWidth The original width of the matrix.
 * @param a Pointer to the entries of the matrix.
 * @param result Pointer to the results array of the matrix.
 */
template <typename T>
void cudaTranspose(size_t n, size_t oldWidth, T* a, T* result);

/**
 * @brief Zeroes a CUDA-allocated array.
 * @tparam T Type of element in the array.
 * @param n The size of the array.
 * @param a Pointer to the array.
 */
template <typename T>
void cudaZeroArray(size_t n, T* a);

/**
 * @brief Creates an array that represents an identity matrix of dimension 'width'.
 * @tparam T The type of element in the array.
 * @param n The number of elements in array 'a'.
 * @param width The width of the matrix this array represents.
 * @param a Pointer to the array.
 */
template <typename T>
void cudaIdentityArray(size_t n, size_t width, T* a);

}

#endif //TITANPLUSPLUS_CUDAMATH_H
