cmake_minimum_required(VERSION 2.8)

project(HafmanAlgorithm)

add_executable(hafman
                src/main.cpp
                src/vector_bb.h
				src/rtv_data_types.cpp
				src/rtv_data_types.h
				src/alg_tools.cpp
				src/alg_tools.h
                )

include_directories(
                    src/
                    tests/
                    CatchInclude/single_include/
                    )
                    
add_executable(test_vector
                tests/test_vector_bb.cpp
                src/vector_bb.h
				src/rtv_data_types.cpp
				src/rtv_data_types.h
                CatchInclude/single_include/catch.hpp
               )