//
// Created by Bryn McKerracher on 21/10/2021.
//

#ifndef TITANPLUSPLUS_MATRIX_TPP
#define TITANPLUSPLUS_MATRIX_TPP

#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(int x, int y) {
    //Set array metadata.
    entriesSize = x * y;
    width = x;

    //Allocate CUDA memory.
    cudaMallocManaged(&entries, entriesSize * sizeof(T));
}

template <typename T>
Matrix<T>::Matrix(const std::string& str) {
    size_t rowStart = str.find_first_of('[');

    //If no row start found, it's a vector.
    if (rowStart == std::string::npos) {
        width = 1;
    }
    else {
        width = numericParse(str.substr(1, str.find_first_of(']') - 1)).size();
    }

    auto numericEntries = numericParse(str);
    entriesSize = numericEntries.size();

    //Allocate CUDA memory.
    cudaMallocManaged(&entries, entriesSize * sizeof(T));

    //Init entries
    for (size_t i = 0; i < entriesSize; ++i) {
        entries[i] = numericEntries[i];
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    cudaFree(&entries);
}

template <typename T>
Matrix<T> Matrix<T>::zero(int x, int y) {
    Matrix<T> zeroed(x, y);
    CudaMath::cudaZeroArray(zeroed.entriesSize, zeroed.entries);
    return zeroed;
}

template <typename T>
Matrix<T> Matrix<T>::identity(int n) {
    Matrix<T> identity(n, n);
    CudaMath::cudaIdentityArray(identity.entriesSize, n, identity.entries);
    return identity;
}

template <typename T>
T& Matrix<T>::operator()(int x, int y) {
    return entries[x + y * width];
}

template <typename T>
const T& Matrix<T>::operator()(int x, int y) const {
    return entries[x + y * width];
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &rhs) const {
    return entriesSize == rhs.entriesSize && width == rhs.width && CudaMath::cudaEqual<T>(entriesSize, entries, rhs.entries);
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> transpose(this->entriesSize / this->width, this->width);
    CudaMath::cudaTranspose(entriesSize, this->width, entries, transpose.entries);
    return transpose;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const {
    Matrix<T> sum(this->width, this->entriesSize / this->width);
    CudaMath::cudaAdd(entriesSize, this->entries, rhs.entries, sum.entries);
    return sum;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &rhs) const {
    Matrix<T> result(this->width, this->entriesSize / this->width);
    CudaMath::cudaSubtract(entriesSize, this->entries, rhs.entries, result.entries);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &scalar) const {
    Matrix<T> product(this->width, this->entriesSize / this->width);
    CudaMath::cudaScalarMultiply(entriesSize, this->entries, scalar, product.entries);
    return product;
}

template <typename T>
std::string Matrix<T>::toString() const {
    std::stringstream stream;
    stream << "[";
    for (size_t i = 0; i < entriesSize; ++i) {
        if (i > 0 and i % width == 0) stream << "] [";
        stream << entries[i];
        if (i % width != width - 1) {
            stream << ", ";
        }
    }
    stream << "]";
    return stream.str();
}

template<typename T>
std::vector<T> Matrix<T>::numericParse(const std::string &string) const {
    std::vector<T> numericEntries;
    size_t start = 0;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] >= '0' && string[i] <= '9') {
            start = i;
            while (i < string.size() && (string[i] >= '0' && string[i] <= '9') || (i + 1 < string.size() && string[i] == '.' && string[i + 1] >= '0' && string[i + 1] <= '9')) {
                ++i;
            }
            std::stringstream stream(string.substr(start, i - start));
            T value;
            stream >> value;
            numericEntries.push_back(value);
        }
    }
    return numericEntries;
}

#endif //TITANPLUSPLUS_MATRIX_TPP
