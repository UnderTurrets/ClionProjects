cmake_minimum_required(VERSION 3.23)
project(leetcode)

set(CMAKE_CXX_STANDARD 17)
INCLUDE_DIRECTORIES(/home/xu736946693/Desktop/myAlgorithm/lib)
add_executable(leetcode main.cpp)
TARGET_LINK_LIBRARIES(leetcode /home/xu736946693/Desktop/myAlgorithm/build/bin/libmyAlgorithm_shared.so )


