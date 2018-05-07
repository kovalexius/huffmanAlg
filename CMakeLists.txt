cmake_minimum_required(VERSION 2.8)

project(HafmanAlgorithm)

add_executable(hafman
                src/main.cpp
                src/vector_bb.h
				src/element.h
                )

include_directories(
                    src/
                    tests/
                    CatchInclude/single_include/
                    )
                    
add_executable(test_vector
                tests/test_vector_bb.cpp
                src/vector_bb.h
				src/element.h
                CatchInclude/single_include/catch.hpp
               )