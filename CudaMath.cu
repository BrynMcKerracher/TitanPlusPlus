/**
 * @file CudaMath.cu
 * @author Bryn McKerracher
 * @date 20/10/2021
 * @brief Contains Cuda array math for matrix operations.
 */

#include "CudaMath.h"

/**
 * @brief Returns an epsilon value for either float or double.
 * @tparam T The floating point type to get epsilon for.
 * @return The epsilon value for the given floating point type.
 */
template <typename T>
static __device__ T epsilon();

template<> __device__ float epsilon<float>() {
    return FLT_EPSILON;
}

template<> __device__ double epsilon<double>() {
    return DBL_EPSILON;
}

const unsigned BlockSize = 1024; ///< Number of threads per GPU block.

/**
 * @brief Gets the num of thread blocks for a given CUDA array size.
 * @param n The size of the array to get thread blocks sizing for.
 * @return The number of thread blocks for a CUDA operation on an array of size n.
 */
inline size_t GetNumBlocks(size_t n) {
    return (n + BlockSize - 1) / BlockSize;
}

/**
 * Function expects that all arrays have already been allocated and that each array has n elements.
 * No bounds checking is performed.
 *
 * @brief Performs an add operation on each pair of elements from a and b, stores the result in sum.
 * @tparam T The type of elements in the arrays.
 * @param n The number of elements in each array.
 * @param a Pointer to the first summand.
 * @param b Pointer to the second summand.
 * @param sum Pointer to the sum.
 */
template <typename T>
__global__ void deviceAdd(size_t n, T* a, T* b, T* sum) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        sum[i] = a[i] + b[i];
    }
}

/**
 * Function expects that all arrays have already been allocated and that each array has n elements.
 * No bounds checking is performed.
 *
 * @brief Performs a subtraction operation on each pair of elements from a and b, stores the result in result.
 * @tparam T The type of elements in the arrays.
 * @param n The number of elements in each array.
 * @param a Pointer to the minuend.
 * @param b Pointer to the second subtrahend.
 * @param result Pointer to the results array.
 */
template <typename T>
__global__ void deviceSubtract(size_t n, T* a, T* b, T* result) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        result[i] = a[i] - b[i];
    }
}

/**
 * Function expects that all arrays have already been allocated and that each array has n elements.
 * No bounds checking is performed.
 *
 * @brief Performs pointwise scalar multiplication of a by b, and stores the results in result.
 * @tparam T The type of the elements in a, either float or double.
 * @param n The size of the array in a.
 * @param a Pointer to the array being multiplied.
 * @param b Scalar to multiply by.
 * @param result Pointer to result array.
 */
template <typename T>
__global__ void deviceScalarMultiply(size_t n, T* a, T b, T* result) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        result[i] = a[i] * b;
    }
}

/**
 * @brief Performs an element-wise comparison between the provided arrays, and sets equal accordingly.
 * @tparam T The types of elements of each array.
 * @param n The number of elements in each array.
 * @param a The first array.
 * @param b The second array.
 * @param equal Pointer to the result flag.
 */
template <typename T>
__global__ void deviceEqual(size_t n, T* a, T* b, bool* equal) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        if ((a[i] > b[i] ? a[i] - b[i] : b[i] - a[i]) > (a[i] < b[i] ? a[i] : b[i]) * epsilon<T>()) {
            *equal = false;
            return;
        }
    }
}

/**
 * @brief Calculates the transpose of a matrix.
 * @tparam T The types of elements in the matrix.
 * @param n The number of elements in the matrix.
 * @param oldWidth The original width of the matrix.
 * @param a Pointer to the entries of the matrix.
 * @param result Pointer to the results array of the matrix.
 */
template <typename T>
__global__ void deviceTranspose(size_t n, size_t oldWidth, T* a, T* result) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        const size_t x = i % oldWidth;
        const size_t y = i / oldWidth;
        result[y + x * (n / oldWidth)] = a[i];
    }
}

template <typename T>
__global__ void deviceZeroArray(size_t n, T* a) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        a[i] = 0;
    }
}

template <typename T>
__global__ void deviceIdentityArray(size_t n, size_t width, T* a) {
    for (size_t i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x) {
        a[i] = (i % width == i / width) ? 1 : 0;
    }
}

template <typename T>
void CudaMath::cudaAdd(size_t n, T *a, T *b, T *sum) {
    deviceAdd<T><<<GetNumBlocks(n), BlockSize>>>(n, a, b, sum);
    cudaDeviceSynchronize();
}

template <typename T>
void CudaMath::cudaSubtract(size_t n, T *a, T *b, T *result) {
    deviceSubtract<T><<<GetNumBlocks(n), BlockSize>>>(n, a, b, result);
    cudaDeviceSynchronize();
}

template <typename T>
void CudaMath::cudaScalarMultiply(size_t n, T *a, T b, T *result) {
    deviceScalarMultiply<<<GetNumBlocks(n), BlockSize>>>(n, a, b, result);
    cudaDeviceSynchronize();
}

template <typename T>
bool CudaMath::cudaEqual(size_t n, T *a, T *b) {
    //Allocate bool
    bool* deviceEqualFlag = nullptr;
    cudaMallocManaged(&deviceEqualFlag, sizeof (bool));
    *deviceEqualFlag = true;
    //Create host flag
    bool hostEqualFlag = false;
    //Run CUDA check and sync threads
    deviceEqual<T><<<GetNumBlocks(n), BlockSize>>>(n, a, b, deviceEqualFlag);
    cudaDeviceSynchronize();
    //Copy flag from device to host
    cudaMemcpy(&hostEqualFlag, deviceEqualFlag, sizeof (bool), cudaMemcpyDeviceToHost);
    //Free memory
    cudaFree(deviceEqualFlag);
    return hostEqualFlag;
}

template <typename T>
void CudaMath::cudaTranspose(size_t n, size_t oldWidth, T* a, T* result) {
    deviceTranspose<T><<<GetNumBlocks(n), BlockSize>>>(n, oldWidth, a, result);
    cudaDeviceSynchronize();
}

template <typename T>
void CudaMath::cudaZeroArray(size_t n, T *a) {
    deviceZeroArray<<<GetNumBlocks(n), BlockSize>>>(n, a);
    cudaDeviceSynchronize();
}

template <typename T>
void CudaMath::cudaIdentityArray(size_t n, size_t width, T *a) {
    deviceIdentityArray<<<GetNumBlocks(n), BlockSize>>>(n, width, a);
    cudaDeviceSynchronize();
}

///Forward declarations
//Add
template void CudaMath::cudaAdd<float>(size_t n, float* a, float* b, float* sum);
template void CudaMath::cudaAdd<double>(size_t n, double* a, double* b, double* sum);

//Subtract
template void CudaMath::cudaSubtract<float>(size_t n, float* a, float* b, float* result);
template void CudaMath::cudaSubtract<double>(size_t n, double* a, double* b, double* result);

//Scalar multiply
template void CudaMath::cudaScalarMultiply<float>(size_t n, float* a, float b, float* result);
template void CudaMath::cudaScalarMultiply<double>(size_t n, double* a, double b, double* result);

//Equal
template bool CudaMath::cudaEqual<float>(size_t n, float* a, float* b);
template bool CudaMath::cudaEqual<double>(size_t n, double* a, double* b);

//Transpose
template void CudaMath::cudaTranspose<float>(size_t n, size_t oldWidth, float* a, float* result);
template void CudaMath::cudaTranspose<double>(size_t n, size_t oldWidth, double* a, double* result);

//Zero array
template void CudaMath::cudaZeroArray<float>(size_t n, float* a);
template void CudaMath::cudaZeroArray<double>(size_t n, double* a);

//Identity array
template void CudaMath::cudaIdentityArray<float>(size_t n, size_t width, float* a);
template void CudaMath::cudaIdentityArray<double>(size_t n, size_t width, double* a);