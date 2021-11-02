#ifndef TITANPLUSPLUS_MATRIX_CU
#define TITANPLUSPLUS_MATRIX_CU

/**
 * @file Matrix.h
 * @author Bryn McKerracher
 * @date 19/10/2021
 * @brief The Matrix class. Uses CUDA methods to perform matrix operations.
 */

#include <cuda_runtime.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "CudaMath.h"

/**
 * Uses CUDA computation to provide fast matrix operations.
 *
 * @note All CUDA operations are grid-stride loops and are thus optimised for large array/matrix operations.
 *
 * @warning Currently, the matrix class strictly requires a CUDA GPU and will not function
 * without one. It is intended that further updates will also allow CPU matrices if no CUDA GPU is found.
 *
 * @brief Represents a double precision Matrix.
 * @class Matrix
 */
template <typename T>
class Matrix {
public:
    /**
     * @note This is a CUDA device function.
     * @brief Creates a zero-intialised matrix with the given dimensions.
     * @param x The width of the new matrix.
     * @param y The height of the new matrix.
     */
     Matrix(int x, int y);

     /**
      * @brief Creates a matrix from numeric values in a string.
      * @param str The string to read matrix entries from.
      */
     Matrix(const std::string& str);

    /**
     * @brief Frees the allocated CUDA memory on the GPU.
     */
    ~Matrix();

    /**
     * @brief Creates an matrix of zeroes with the given dimensions.
     * @param x The width of the new matrix.
     * @param y The height of the new matrix.
     * @return An matrix of zeroes with dimensions [x, y].
     */
    static Matrix zero(int x, int y);

    /**
     * @brief Creates an identity matrix with the given dimensions.
     * @param n The width and height of the new matrix.
     * @return An identity matrix with dimensions [x, y].
     */
    static Matrix identity(int n);

    /**
     * @brief Creates a reference to a specific entry for setting a value.
     * @param x The x coordinate of the entry.
     * @param y The y coordinate of the entry.
     */
    T& operator()(int x, int y);

    /**
     * @brief Creates a const reference to a specific entry for getting a value.
     * @param x The x coordinate of the entry.
     * @param y The y coordinate of the entry.
     */
    const T& operator()(int x, int y) const;

    /**
     * @brief Performs an element-wise equality comparison with another matrix.
     * @param rhs The matrix to compare against.
     * @return True if both matrices are element-wise equal, otherwise false.
     */
    bool operator==(const Matrix& rhs) const;

    /**
     * @brief Calculates the transpose of this matrix.
     * @return The transpose of this matrix.
     */
    Matrix transpose() const;

    /**
     * @brief Creates a matrix which is the sum of this and rhs.
     * @param rhs The right-hand operand of the matrix add operation.
     * @return Returns the sum of this and rhs.
     */
     Matrix operator+(const Matrix& rhs) const;

     /**
      * @brief Creates a matrix which is the difference between this matrix and rhs.
      * @param rhs The right-hand operand of the matrix subtraction operation.
      * @return Returns the result of this minus rhs.
      */
     Matrix operator-(const Matrix& rhs) const;

     /**
      * @brief Creates a copy of this matrix multiplied by a scalar.
      * @param scalar A scalar to multiply this matrix by.
      * @return The product of this matrix and the provided scalar.
      */
     Matrix operator*(const T& scalar) const;

     /**
      * @brief Creates a human-readable string representation of the matrix.
      * @return A human-readable string representing the matrix.
      */
     std::string toString() const;
protected:
    T* entries = nullptr;   ///< The matrix's entries.
    size_t entriesSize = 0; ///< The number of entries.
    int width = 0;          ///< The width of the matrix.

    /**
     * @brief
     * @param string A string of values to be parsed from.
     * @return A vector of Ts parsed from the garbage string.
     */
    std::vector<T> numericParse(const std::string& string) const;
};

//Forward declarations of matrix types.
typedef Matrix<double> MatrixD; ///< Common matrix type using double precision entries.
typedef Matrix<float>  MatrixF; ///< Common matrix type using single precision entries for TENSOR cores.

#include "Matrix.tpp"

#endif //TITANPLUSPLUS_MATRIX_CU
