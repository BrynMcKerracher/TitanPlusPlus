cmake_minimum_required(VERSION 3.20)
project(TitanPlusPlus LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

add_library(TitanCUDA STATIC
    CudaMath.cu
    CudaMath.h
)

set_target_properties(TitanCUDA PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories(TitanCUDA PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/include")
#option(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/bin/nvcc.exe")

add_executable(TitanPlusPlus
    common.h
    Batch.cpp
    Batch.h
    Ops.cpp
    Ops.h
    Debug.cpp
    Debug.h
    Value.cpp
    Value.h
    VM.cpp
    VM.h
    Compiler.cpp
    Compiler.h
    Scanner.cpp
    Scanner.h
    Token.cpp
    Token.h
    Parser.cpp
    Parser.h
    Value.tpp
    main.cpp
    Matrix.h
    Matrix.tpp
        Local.cpp Local.h)

target_link_libraries(TitanPlusPlus PUBLIC TitanCUDA)
target_include_directories(TitanPlusPlus PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/include")

#Google test suite
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(TitanEngine STATIC
        common.h
        Batch.cpp
        Batch.h
        Ops.cpp
        Ops.h
        Debug.cpp
        Debug.h
        Value.cpp
        Value.h
        VM.cpp
        VM.h
        Compiler.cpp
        Compiler.h
        Scanner.cpp
        Scanner.h
        Token.cpp
        Token.h
        Parser.cpp
        Parser.h
        Value.tpp
        Matrix.h
        Matrix.tpp
        Local.cpp Local.h)

target_include_directories(TitanEngine PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/include")

add_executable(
    TestSuite
    testing/matrix/MatrixTesting.h
    testing/matrix/MatrixTesting.cpp
    testing/variable/VariableTest.h
    testing/variable/VariableTest.cpp
)

target_link_libraries(TestSuite PUBLIC TitanCUDA TitanEngine gtest_main)
target_include_directories(TestSuite PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/include")

include(GoogleTest)
gtest_discover_tests(TestSuite)

# CUDA Profiling test suite
add_executable(MatrixSpeed
    Matrix.h
    Matrix.tpp
    CudaMath.h
    CudaMath.cu
    testing/speed/main.cpp
)

target_include_directories(MatrixSpeed PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.4/include")