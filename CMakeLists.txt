cmake_minimum_required(VERSION 3.19)
project(TitanPlusPlus)

set(CMAKE_CXX_STANDARD 20)

add_executable(TitanPlusPlus
    main.cpp
    common.h
    Batch.cpp
    Batch.h
    Ops.cpp
    Ops.h
    Debug.cpp
    Debug.h
    Memory.cpp
    Memory.h
    Memory.tpp
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
        Object.cpp Object.h Value.tpp)