cmake_minimum_required(VERSION 3.9)
project(PQCMiner)

set(CMAKE_CXX_STANDARD 11)

include(CMakeHelp.cmake)

CheckLibrary()

add_executable(PQCMiner
        main.cpp PQCMiner.cpp PQCMiner.h cube_hash.h cubehash_impl.h hash.h Block.cpp Block.h types.h Buffer.cpp Buffer.h)

LinTargetLibrary(PQCMiner)
